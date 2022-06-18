#pragma once

namespace ECS
{
	class ECSManager;
}
namespace System
{
	struct ISystem
	{
		using query = std::tuple<>;

		void Update() noexcept {}
	};

	struct System : ISystem
	{
		using entity = Entity::Entity;

		System(const System&) noexcept = delete;

		System(ECS::ECSManager& _gameMgr) noexcept;

		ECS::ECSManager& m_gameMgr;
	};
}