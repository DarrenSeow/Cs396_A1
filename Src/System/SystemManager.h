#pragma once
/******************************************************************************
filename: SystemManager.h
author: Seow Jun Hao Darren seow.j@digipen.edu
Project: Cs396_A1
Description:
Contains the declaration of System Manager and SystemInfo class
******************************************************************************/

namespace System
{
	struct SystemManager final
	{
		struct SystemInfo
		{
			using callRun = void(System&);
			std::unique_ptr<System> m_system;
			callRun*				m_callRun;
		};
		SystemManager(const SystemManager&) noexcept = delete;
		SystemManager() noexcept = default;


		template<typename T_System>
		requires( std::derived_from<T_System,System>)
		T_System& RegisterSystem(ECS::ECSManager& _ecsMgr) noexcept;
		void Run() noexcept;

		std::vector<SystemInfo> m_systems;
	};
}