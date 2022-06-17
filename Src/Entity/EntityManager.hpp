#include "EntityManager.h"
namespace Entity
{
	inline Archetype::Archetype& EntityManager::GetOrCreateArchetype(std::span < const Component::ComponentInfo* const> _span) noexcept
	{
		ECS_Tools::Bits Query;
		for (const auto& componentInfo : _span)
		{
			assert(componentInfo->m_uid != ECS_Utility::invalid_component_info_value);
			Query.SetBit(componentInfo->m_uid);
		}

		//make sure entity component exist 

		for (auto& E : m_archetypeBits)
		{
			if (E.Compare(Query))
			{
				const auto index = static_cast<std::size_t>(&E - m_archetypeBits.data());
				return *m_archetypes[index];
			}
		}

		//create archetype...

		m_archetypes.push_back(std::make_unique<Archetype::Archetype>(_span, Query, *this));

		m_archetypeBits.push_back(Query);

		return *m_archetypes.back();
	}
	inline Entity EntityManager::AllocNewEntity(ECS_Utility::EntityIndex _index, Archetype::Archetype& _archetype) noexcept
	{
		assert(m_emptyHead < ECS_Utility::max_num_of_entity);
		auto entityIndex = m_emptyHead;
		auto& record = m_entityRecords[entityIndex];
		m_emptyHead = record.m_index;
		
		record.m_index = _index;
		record.m_archetype = &_archetype;

		return
		{
			.m_index = entityIndex,
			.m_validation = record.m_validation
		};

	}
	inline EntityManager::EntityManager() noexcept
	{
		//linked list of empty entries
		for (int i = 0; i < ECS_Utility::max_num_of_entity - 2; ++i)
		{
			m_entityRecords[i].m_index = i + 1;
		}
	}
	inline std::vector<Archetype::Archetype*> EntityManager::Search(std::span<const Component::ComponentInfo* const> _types) const noexcept
	{
		ECS_Tools::Bits Query;
		for (const auto& info : _types)
		{
			Query.SetBit(info->m_uid);
		}

		std::vector<Archetype::Archetype*> archetypesFound;
		for (auto& bits : m_archetypeBits)
		{
			if (bits.Compare(Query))
			{
				const auto index = static_cast<std::size_t>(&bits - &m_archetypeBits[0]);
				archetypesFound.push_back(m_archetypes[index].get());
			}
		}
		return std::move(archetypesFound);
	}
	inline std::vector<Archetype::Archetype*> EntityManager::Search(const Query::Query& _query) const noexcept
	{
				
		std::vector<Archetype::Archetype*> archetypesFound;
		for (auto& bits : m_archetypeBits)
		{
			if (_query.Compare(bits))
			{
				const auto index = static_cast<std::size_t>(&bits - &m_archetypeBits[0]);
				archetypesFound.push_back(m_archetypes[index].get());
			}
		}
		return std::move(archetypesFound);
	}
	inline const EntityRecord& EntityManager::GetEntityDetails(Entity& _ent) noexcept
	{
		EntityRecord& record = m_entityRecords[_ent.m_index];
		assert(_ent.m_validation == record.m_validation);

		return record;
	}
	template<typename... Components>
	inline Archetype::Archetype& EntityManager::GetOrCreateArchetype() noexcept
	{
		static_assert(((std::is_same_v<std::decay_t<Components>, Entity> == false)&&...));

		static constexpr auto ComponentList = std::array{ &Component::ComponentInfo_v<Entity>,&Component::ComponentInfo_v<Components>... };

		return GetOrCreateArchetype(ComponentList);
	}

	template<Tools::is_void_Fn CallBackType>
	inline Entity EntityManager::CreateEntity(CallBackType&& _function) noexcept
	{
		using func_traits = Tools::Fn_Traits<CallBackType>;

		return[&]<typename ... Components>(std::tuple<Components...>*) constexpr noexcept
		{
			auto& archetype = GetOrCreateArchetype<Components...>();

			assert(archetype.m_bits.GetBit(Component::ComponentInfo_v<Components>.m_uid) && ...);
			//add entity to its respective archetype
			const auto EntityIndexInPool = archetype.AppendEntity();
			//add entity to the global pool

			const auto entity = AllocNewEntity(EntityIndexInPool, archetype);
			archetype.GetComponent<Entity>(EntityIndexInPool) = entity;

			if constexpr (false == std::is_same_v<ECS_Utility::EmptyLambda, CallBackType>)
				_function(archetype.GetComponent<std::remove_reference_t<Components>>(EntityIndexInPool)...);
			return entity;
		}(Tools::cast_null_tuple_v<func_traits::Args_Tuple>);
	}
	template<Tools::is_void_Fn CallBackType>
	inline bool EntityManager::FindEntity(Entity _ent, CallBackType&& Function) const noexcept
	{
		if (_ent.IsAlive()) return false;

		auto& record = m_entityRecords[_ent.m_index];

		if (record.m_validation == _ent.m_validation)
		{
			if constexpr (!std::is_same_v<CallBackType, ECS_Utility::EmptyLambda>)
			{
				[&] <typename... Components>(std::tuple<Components...>*) constexpr noexcept
				{
					Function(record.m_archetype->GetComponent<std::remove_reference_t<Components>>(record.m_index)...);
				}(Tools::cast_null_tuple_v<Tools::Fn_Traits<CallBackType>::Args_Tuple>);
			}
			return true;
		}
		return false;
	}
	template<typename ...Components>
	inline std::vector<Archetype::Archetype*> EntityManager::Search() const noexcept
	{
	
	//	static_assert(((std::is_same_v<<Components>, Entity>)&&...));
		if constexpr (((std::is_same_v<Components, Entity>)&&...))
		{
			static constexpr auto ComponentList = std::array{ &Component::ComponentInfo_v<Components>... };
			return Search(ComponentList);
		}
		
		static constexpr auto ComponentList = std::array{ &Component::ComponentInfo_v<Entity>, &Component::ComponentInfo_v<Components>... };
		return Search(ComponentList);
		
	}
}