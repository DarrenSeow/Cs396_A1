#pragma once
/******************************************************************************
filename: ComponentManager.hpp
author: Seow Jun Hao Darren seow.j@digipen.edu
Project: Cs396_A1
Description:
Contains the definition of ComponentManager function
******************************************************************************/

namespace Component
{
	template<typename T>
	void ComponentManager::RegisterComponent() noexcept
	{
		constexpr auto& uid = ComponentInfo_v<T>.m_uid;

		if (uid != ECS_Utility::invalid_component_info_value)
		{
			return;
		}
		uid = m_uniqueNumOfComponent++;
	}
	template<typename ...Components>
	inline void ComponentManager::RegisterComponents() noexcept
	{
		(RegisterComponent<Components>(), ...);
	}

}
