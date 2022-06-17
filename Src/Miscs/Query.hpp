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
	template<Tools::has_functor Function>
	constexpr void Query::AddQueryFromFunction(Function&&) noexcept
	{
		[&] <typename ...TuplesArgs>(std::tuple<TuplesArgs... >>* )
		{

			(
				[&]<typename Type>(std::tuple<Type>*)
				{
					
				if constexpr (std::is_pointer_v<Type>)
				{
					m_oneOf.AddFromComponents<Type>();
				}
				else if (constexpr(std::is_reference_v<Type>))
				{
					m_must.AddFromComponents<Type>();
				}
				else
				{
					static_assert(false,"not enough info");
				}
				}(Tools::make_null_tuple_from_args_v<TuplesArgs>)

			, ...);

			
		}(Tools::cast_null_tuple_v<Tools::Fn_Traits<Function>::Parameters_t>);
	}

	template<typename ... Queries>
	void Query::AddQueryFromTuple(std::tuple<Queries...>*) noexcept
	{
		([&]template<typename...>class T_QType, typename... Components > (QType<Components...>*)
		{
			using TQuery = T_QType<Component...>;
			if constexpr (std::is_same_v<TQuery, Must<Components...>>)
			{
				m_must.AddFromComponents<Components...>();
			}
			else if constexpr (std::is_same_v< TQuery, OneOf<Components...>>)
			{
				m_oneOf.AddFromComponents<Components...>();
			}
			else if constexpr (std::is_same_v<TQuery, NoneOf<Components...>>)
			{
				m_noneOf.AddFromComponents<Components...>();
			}
			else
			{
				static_assert(false, "Wrong query used");
			}
		}(cast_null_tuple_v<Queries>)
		, ...);
	}
}