#pragma once
/******************************************************************************
filename: Bits.h
author: Seow Jun Hao Darren seow.j@digipen.edu
Project: Cs396_A1
Description:
Contains the declartion of bits function
******************************************************************************/

namespace ECS_Tools
{
	struct Bits final
	{
		void SetBit(const std::int64_t _index) noexcept;
		constexpr bool GetBit(const std::int64_t _index) const noexcept;
		constexpr bool Compare(const Bits& _bits) const noexcept;
		
		template<typename ... Component>
		void AddFromComponents() noexcept;
		std::array<std::int64_t, (ECS_Utility::max_num_of_component - 1) / 64 + 1> m_bits{0};
	};
}
