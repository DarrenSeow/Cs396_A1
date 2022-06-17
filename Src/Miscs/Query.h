#pragma once

namespace Query
{
	template<typename... Component>
	struct Must final{};
	template<typename... Component>
	struct OneOf final {};

	template<typename ...Component>
	struct NoneOf final {};

	struct Query final
	{
		ECS_Tools::Bits m_must;
		ECS_Tools::Bits m_oneOf;
		ECS_Tools::Bits m_noneOf;

		bool Compare(const ECS_Tools::Bits& _bits) const noexcept;

		template<Tools::has_functor Function>
		constexpr void AddQueryFromFunction(Function&&) noexcept;
		template<typename ...Queries>
		void AddQueryFromTuple(std::tuple<Queries...>*) noexcept;
	};
}
#include "Query.hpp"