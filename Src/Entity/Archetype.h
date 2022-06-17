#pragma once

namespace Entity
{
	class EntityManager;
	union Entity;
}
namespace Archetype
{

	struct Archetype final
	{

		using info_array = std::array<const Component::ComponentInfo* const, ECS_Utility::max_num_of_component_per_entity>;

		Archetype() = delete;

		Archetype(const std::span<const Component::ComponentInfo* const >& _infos,
			const ECS_Tools::Bits& _bits,
			Entity::EntityManager& _entityMgr) noexcept;

		
		
		ECS_Utility::EntityIndex AppendEntity() noexcept;

		
		void UpdateStructuralChange() noexcept;
		void DestroyEntity(Entity::Entity _entity) noexcept;

		Entity::EntityManager& m_entityMgr;
		ECS_Tools::Bits m_bits{};
		std::vector<Entity::Entity> m_entityToDelete{};
		std::vector<Component::ComponentInfo*> m_infos{};
		Pool m_componentPool;
		info_array m_infoData;
		int m_processRef{ 0 };
	};
}

#include "Archetype.hpp"