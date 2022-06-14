#pragma once


namespace Component
{
	class ComponentManager
	{
		inline static ECS_Utility::ComponentUID m_uniqueNumOfComponent;
	public:
		ComponentManager() = default;

		ComponentManager(const ComponentManager& _mgr) = delete;
		ComponentManager& operator=(const ComponentManager& _mgr) = delete;

		ComponentManager& operator=(const ComponentManager&& _mgr) noexcept = delete;
		ComponentManager(const ComponentManager&& _mgr) = delete;
		template<typename Component>
		void RegisterComponent() noexcept;

		template<typename ... Components>
		void RegisterComponents() noexcept;

	};


}
#include "ComponentManager.hpp"