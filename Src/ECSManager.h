#pragma once

namespace ECS
{
	class ECSManager
	{
		std::unique_ptr<Entity::EntityManager> m_entityMgr;
		std::unique_ptr<Component::ComponentManager> m_componentMgr;
		std::unique_ptr<System::SystemManager> m_systemMgr;
	public:
		ECSManager() noexcept;

		ECSManager(ECSManager&) = delete;
		ECSManager(ECSManager&&) = delete;

		ECSManager& operator=(ECSManager&) = delete;
		ECSManager& operator=(ECSManager&&) = delete;
		

	};
}