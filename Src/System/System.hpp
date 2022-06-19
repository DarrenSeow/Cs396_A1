#pragma once
/******************************************************************************
filename: System.hpp
author: Seow Jun Hao Darren seow.j@digipen.edu
Project: Cs396_A1
Description:
Contains the definition of System class
******************************************************************************/
namespace System
{
	inline System::System(ECS::ECSManager& _ecsMgr) noexcept
		:m_ecsMgr{ _ecsMgr }
	{

	}
}