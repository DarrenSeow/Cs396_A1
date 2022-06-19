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

		//template<typename Function>
		//requires Tools::is_callable_v<Function>
		//template<Tools::has_functor Function>
		template<typename Function>
		void AddQueryFromFunction(Function&&) noexcept;
		template<typename ...Queries>
		void AddQueryFromTuple(std::tuple<Queries...>*) noexcept;

		template<typename T_Component>
		void InnerUnpackFromFunction(std::tuple<T_Component>*)
		{
			if constexpr (std::is_pointer_v<T_Component>)
			{
				m_oneOf.SetBit(Component::ComponentInfo_v<std::remove_pointer_t<T_Component>>.m_uid);
			}
			else if constexpr (std::is_reference_v<T_Component>)
			{
				m_must.SetBit(Component::ComponentInfo_v<T_Component>.m_uid);
			}
			else
			{
				static_assert(std::false_type<T_Component>::value, "Wrong query //used");
			}
		}
	};
}