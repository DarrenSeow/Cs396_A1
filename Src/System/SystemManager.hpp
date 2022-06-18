#pragma once

namespace System
{
	namespace details
	{
		template<typename UserSystem>
		requires( std::derived_from<UserSystem, System>)
		struct CompletedSystem final : UserSystem
		{
			CompletedSystem(ECS::ECSManager& _ecsMgr) noexcept :
				UserSystem{ _ecsMgr }
			{

			}
			CompletedSystem() noexcept = delete;

			void Run() noexcept
			{
				if constexpr (&UserSystem::Update != &System::Update)
				{
					System::Update();
				}
				else
				{
					Query::Query query;
					query.AddQueryFromTuple(Tools::cast_null_tuple_v<UserSystem::query>);
					query.AddQueryFromTuple(*this);
					UserSystem::m_ecsMgr.Foreach(UserSystem.Search(query), *this);
				}
			}


		};
	}
	template<typename T_System>
		requires(std::derived_from<T_System, System>)
	T_System& SystemManager::RegisterSystem(ECS::ECSManager& _escMgr)  noexcept
	{
		m_systems.push_back(SystemInfo
			{
				.m_System = std::make_unique < details::CompletedSystem<T_System>(_ecs),
				.m_callRun = [](System& _system) noexcept
			{
				static_cast<details::CompletedSystem<System>&>(_system).Run();
			}
			});

		return *static_cast<T_System*>(m_systems.back().m_system.get());

	}

	void SystemManager::Run() noexcept
	{
		for (const auto& system : m_systems)
		{
			system.m_callRun(*system.m_system.get());
		}
	}
}