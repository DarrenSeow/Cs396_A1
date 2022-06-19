#pragma once

namespace Query
{
	bool Query::Compare(const ECS_Tools::Bits& _bits) const noexcept
	{
		std::uint64_t c = 0;
		std::uint64_t a = 0;

		size_t end = _bits.m_bits.size();
		for (size_t i = 0; i < end; ++i)
		{
			if (m_noneOf.m_bits[i] & _bits.m_bits[i])
				return false;
			if ((m_must.m_bits[i] & _bits.m_bits[i]) != m_must.m_bits[i])
				return false;
			c |= (m_oneOf.m_bits[i] & _bits.m_bits[i]);
			a |= (m_oneOf.m_bits[i]);
		}

		// a | c
		//------
		// 0  0 | 1 true
		// 0  1 | x error
		// 1  0 | 0 false
		// 1  1 | 1 true
		assert(!(!a && c));
		return !a || c;
	}
	//template<typename Function>
		//requires Tools::is_callable_v<Function>
	//template<Tools::has_functor Function>
	template<typename Function>
	void Query::AddQueryFromFunction(Function&&) noexcept
	{
		using args = Tools::Fn_Traits<Function>::Args_Tuple;
		

		[&] <typename ...TuplesArgs>(std::tuple<TuplesArgs... > *) noexcept
		{
			
			(InnerUnpackFromFunction(Tools::make_null_tuple_from_args_v<TuplesArgs>), ...);

		}(Tools::cast_null_tuple_v<args>);

	}

	template<typename ... Queries>
	void Query::AddQueryFromTuple(std::tuple<Queries...>*) noexcept
	{
		([&]<template<typename...>class T_QType, typename... Components > (T_QType<Components...>*)
		{
			using TQuery = T_QType<Components...>;
			if constexpr  (std::is_same_v<TQuery, Must<Components...>>)
			{
				m_must.AddFromComponents<std::remove_pointer_t<Components...>>();
			}
			else if constexpr (std::is_same_v< TQuery, OneOf<Components...>>)
			{
				m_oneOf.AddFromComponents<std::remove_pointer_t<Components...>>();
			}
			else if constexpr (std::is_same_v<TQuery, NoneOf<Components...>>)
			{
				m_noneOf.AddFromComponents<std::remove_pointer_t<Components...>>();
			}
			else
			{
				static_assert(std::false_type<TQuery>::value, "Wrong query used");
			}
		}(Tools::cast_null_tuple_v<Queries>)
		, ...);
	}
}