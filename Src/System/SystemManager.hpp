#pragma once
/******************************************************************************
filename: SystemManager.h[[
author: Seow Jun Hao Darren seow.j@digipen.edu
Project: Cs396_A1
Description:
Contains the Definition of System and ISystem class
******************************************************************************/
namespace System
{
	namespace details
	{
		template<typename UserSystem>
		requires( std::derived_from<UserSystem, System>)
		struct CompletedSystem final : UserSystem
		{
			__inline CompletedSystem(ECS::ECSManager& _ecsMgr) noexcept :
				UserSystem{ _ecsMgr }
			{

			}
			CompletedSystem() noexcept = delete;

			__inline void Run() noexcept
			{
				//if constexpr (&UserSystem::Update != &System::Update)
				if constexpr (&UserSystem::Update != &System::Update)
				{

					UserSystem::Update();
				}
				else
				{
					Query::Query systemQuery;

					systemQuery.AddQueryFromTuple(Tools::cast_null_tuple_v<UserSystem::query>);
				
					systemQuery.AddQueryFromFunction(*this);
					UserSystem::m_ecsMgr.Foreach(UserSystem::m_ecsMgr.Search(systemQuery),*this);
				}
			}


		};
	}
	template<typename T_System>
		requires(std::derived_from<T_System, System>)
	T_System& SystemManager::RegisterSystem(ECS::ECSManager& _ecsMgr)  noexcept
	{
		m_systems.push_back(SystemInfo
			{
				.m_system = std::make_unique < details::CompletedSystem<T_System>>(_ecsMgr),
				.m_callRun = [](System& _system) noexcept
			{
				static_cast<details::CompletedSystem<T_System>&>(_system).Run();
			}
			}
		);

		return *static_cast<T_System*>(m_systems.back().m_system.get());

	}

	void SystemManager::Run() noexcept
	{
		for (const auto& systeminfo : m_systems)
		{
			systeminfo.m_callRun(*systeminfo.m_system.get());
		}
	}
}