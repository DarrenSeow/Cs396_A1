#pragma once

namespace ECS
{
	ECSManager::ECSManager() noexcept :
		m_entityMgr{std::make_unique<Entity::EntityManager>()},
		m_componentMgr{std::make_unique<Component::ComponentManager>()},
		m_systemMgr{std::make_unique<System::SystemManager>()}
	{
		m_componentMgr->RegisterComponent<Entity::Entity>();
	}
}