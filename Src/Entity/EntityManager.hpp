#include "EntityManager.h"
#pragma once

namespace Entity
{
	Archetype::Archetype& EntityManager::GetOrCreateArchetype(std::span < const Component::ComponentInfo* const> _span) noexcept
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
	inline Entity EntityManager::AllocNewEntity(ECS_Utility::EntityID _index, Archetype::Archetype& _archetype) noexcept
	{
		assert(m_emptyHead < ECS_Utility::max_num_of_entity);
		auto entityIndex = m_emptyHead;
		auto& entry = m_entityRecords[entityIndex];
		m_emptyHead = entry.m_index;
		
		entry.m_index = entityIndex;
		entry.m_archetype = &_archetype;

		return
		{
			.m_index = entityIndex,
			.m_validation = entry.m_validation
		};

	}
	inline EntityManager::EntityManager() noexcept
	{
		//linked list of empty entries
		for (size_t i = 0; i < ECS_Utility::max_num_of_entity - 2; ++i)
		{
			m_entityRecords[i].m_index = i + 1;
		}
	}
	template<typename... Components>
	Archetype::Archetype& EntityManager::GetOrCreateArchetype() noexcept
	{
		static_assert(((std::is_same_v<Components, Entity> == false)&&...));

		static constexpr auto ComponentList = std::array{ &Component::ComponentInfo_v<Entity>,&Component::ComponentInfo_v<Components>... };

		return GetOrCreateArchetype(ComponentList);
	}

	template<Tools::is_void_Fn CallBackType>
	Entity EntityManager::CreateEntity(CallBackType&& _function) noexcept
	{
		using func_traits = Tools::Fn_Traits<CallBackType>;

		return[&]<typename ... Components>(std::tuple<Components...>*) constexpr noexcept
		{
			auto& archetype = GetOrCreateArchetype<Components...>();

			assert(archetype.m_bits.GetBit(Component::ComponentInfo_v<Components>.m_uid) && ...);

			const auto EntityIndexInPool = archetype.AppendEntity();
			//add entity to the global pool

			const auto entity = AllocNewEntity(EntityIndexInPool, archetype);
			archetype.m_componentPool.GetComponent<Entity>(EntityIndexInPool) = entity;

			return entity;
		}(Tools::cast_null_tuple_v<func_traits::Args_Tuple>);


	}
}