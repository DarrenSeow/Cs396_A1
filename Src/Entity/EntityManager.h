#pragma once


namespace Entity
{
	class EntityManager final
	{
		Archetype::Archetype& GetOrCreateArchetype(std::span < const Component::ComponentInfo* const> _span) noexcept;


		template<typename... Components>
		Archetype::Archetype& GetOrCreateArchetype() noexcept;

		Entity AllocNewEntity(ECS_Utility::EntityID _index, Archetype::Archetype& _archetype) noexcept;
	public:

		EntityManager() noexcept;
		template<Tools::is_void_Fn CallBackType>
		Entity CreateEntity(CallBackType&& _function) noexcept;

		std::vector<std::unique_ptr<Archetype::Archetype>> m_archetypes;
		std::vector<ECS_Tools::Bits> m_archetypeBits;
		std::unique_ptr<EntityRecord[]> m_entityRecords = std::make_unique<EntityRecord[]>(ECS_Utility::max_num_of_entity);

		ECS_Utility::EntityIndex m_emptyHead = 0;
	};
}

#include "EntityManager.hpp"