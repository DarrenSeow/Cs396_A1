#pragma once


namespace Entity
{
	class EntityManager final
	{
		Archetype::Archetype& GetOrCreateArchetype(std::span < const Component::ComponentInfo* const> _span) noexcept;


		template<typename... Components>
		Archetype::Archetype& GetOrCreateArchetype() noexcept;

		Entity AllocNewEntity(ECS_Utility::EntityIndex _index, Archetype::Archetype& _archetype) noexcept;

		void SystemDeleteEntity(Entity _entity) noexcept;

		void SystemDeleteEntity(Entity _entity, Entity& _swappedentity) noexcept;

		friend void Archetype::Archetype::UpdateStructuralChange() noexcept;
	public:

		EntityManager() noexcept;

		void DeleteEntity(Entity& _entity) noexcept;

		//template<Tools::is_bool_fn Function>
		template<typename Function>
		requires std::is_same_v<bool, typename Tools::Fn_Traits<Function>::ReturnType_t> && Tools::has_functor<Function>
		void Foreach(const std::vector<Archetype::Archetype*>& _archetypeList, Function&& _func) const noexcept;

		//template<Tools::is_void_Fn Function>
		template<typename Function>
			requires  std::is_same_v<void, typename Tools::Fn_Traits<Function>::ReturnType_t>&& Tools::has_functor<Function>
		void Foreach(const std::vector<Archetype::Archetype*> _list, Function&& _func) const noexcept;
		//template<Tools::is_void_Fn CallBackType>
		template<typename CallBackType>
			requires  std::is_same_v<void, typename Tools::Fn_Traits<CallBackType>::ReturnType_t>
		Entity CreateEntity(CallBackType&& _function) noexcept;
		template<typename... Components>
		Entity CreateEntity() noexcept;
		//template<Tools::is_void_Fn CallBackType>
		template<typename CallBackType>
			requires  std::is_same_v<void, typename Tools::Fn_Traits<CallBackType>::ReturnType_t>
		bool FindEntity(Entity _ent, CallBackType&& Function) const noexcept;

		std::vector<Archetype::Archetype*> Search(std::span < const Component::ComponentInfo* const> _types) const noexcept;

		template<typename ... Components>
		std::vector<Archetype::Archetype*> Search() const noexcept;

		std::vector<Archetype::Archetype*> Search(const Query::Query& _query) const noexcept;
		const EntityRecord& GetEntityDetails(Entity& _ent) noexcept;

		std::vector<std::unique_ptr<Archetype::Archetype>> m_archetypes{};
		std::vector<ECS_Tools::Bits> m_archetypeBits{};
		std::unique_ptr<EntityRecord[]> m_entityRecords = std::make_unique<EntityRecord[]>(ECS_Utility::max_num_of_entity);

		ECS_Utility::EntityIndex m_emptyHead = 0;
	};
}
