#pragma once


namespace ECS_Tools
{
	struct Bits final
	{
		void SetBit(const std::int32_t _index) noexcept;
		constexpr bool GetBit(const std::int32_t _index) const noexcept;
		constexpr bool Compare(const Bits& _bits) const noexcept;
		
		template<typename ... Component>
		void AddFromComponents() noexcept;
		std::array<std::int64_t, (ECS_Utility::max_num_of_component - 1) / ECS_Utility::max_num_of_component + 1> m_bits{};
	};
}

#include "Bits.hpp"