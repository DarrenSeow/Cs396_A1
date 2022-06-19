#pragma once
/******************************************************************************
filename: System.h
author: Seow Jun Hao Darren seow.j@digipen.edu
Project: Cs396_A1
Description:
Contains the declaration of System and ISystem class
******************************************************************************/
namespace ECS
{
	class ECSManager;
}
namespace System
{
	struct ISystem
	{
		using query = std::tuple<Query::Must<Entity::Entity>>;

		void Update() noexcept {}
	};

	struct System : ISystem
	{
		using entity = Entity::Entity;

		System(const System&) noexcept = delete;

		System(ECS::ECSManager& _gameMgr) noexcept;

		ECS::ECSManager& m_ecsMgr;
	};
}