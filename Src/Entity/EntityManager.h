#pragma once


namespace Entity
{
	class EntityManager final
	{

		ECS_Utility::EntityID CreateEntity();

		std::vector<Archetype::Pool > m_archetypes;
		std::unique_ptr<EntityRecord[]> m_entityRecords = std::make_unique<EntityRecord[]>(ECS_Utility::max_num_of_entity);
	};
}