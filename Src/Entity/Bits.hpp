#pragma once


namespace ECS_Tools
{
	inline void Bits::SetBit(const std::int64_t _index) noexcept 
	{
		std::int64_t x = _index / 64;
		std::int64_t y = _index % 64;
		m_bits[x] |= (1ull << y);
	}

	inline constexpr bool Bits::GetBit(const std::int64_t _index) const noexcept
	{
		std::int64_t x = _index / 64;
		std::int64_t y = _index % 64;
		return m_bits[x] & (1ull << y);
	}
	inline constexpr bool Bits::Compare(const Bits& _bits) const noexcept
	{
		for (int i = 0, size = static_cast<int>(m_bits.size()); i < size; ++i)
		{
			if (m_bits[i] != _bits.m_bits[i])
				return false;
		}

		return true;
	}

	template<typename ... Component>
	void Bits::AddFromComponents() noexcept
	{
		((SetBit(Component::ComponentInfo_v<Component>.m_uid)), ...);
	}
}