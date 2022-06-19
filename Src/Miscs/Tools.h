#pragma once
namespace Tools
{
	namespace details
	{
		template<typename T, typename ... Args>
		struct TupleToIndex;

		template<typename T, typename ... Args>
		struct TupleToIndex<T, std::tuple<T, Args...>>
		{
			static constexpr size_t value = 0;
		};

		template<typename T, typename U, typename ... Args>
		struct TupleToIndex<T, std::tuple<U, Args...>>
		{
			static constexpr size_t value = 1 + TupleToIndex<T, Args...> ::value;
		};


		template<typename ReturnType, typename Class, typename ... Args>
		struct Fn_Traits_Info
		{
			using ReturnType_t = ReturnType;
			using Class_t = Class;
			using Args_Tuple = std::tuple<Args...>;

			static constexpr auto args_Count = sizeof...(Args);
		};


	}
	template<typename T, typename Tuple>
	constexpr size_t TupleToIndex_v = details::TupleToIndex<T, Tuple>::value;

	template<typename NonFunction>
	struct Fn_Traits;

	//normal function specialization
	template <typename ReturnType, typename ...Args>
	struct Fn_Traits<ReturnType(Args...)> :
		details::Fn_Traits_Info<ReturnType, void, Args...>
	{

	};


	// function pointer specialization
	template<typename ReturnType, typename ...Args>
	struct Fn_Traits<ReturnType(*)(Args...)> :
		details::Fn_Traits_Info<ReturnType, void, Args...>
	{

	};
	//class function specialization
	template<typename ReturnType, typename Class, typename ...Args>
	struct Fn_Traits<ReturnType(Class::*)(Args...)> :
		details::Fn_Traits_Info<ReturnType, Class, Args...>
	{

	};

	//const class function/lambda specialization
	template<typename ReturnType, typename Class, typename ...Args>
	struct Fn_Traits<ReturnType(Class::*)(Args...) const> :
		details::Fn_Traits_Info<ReturnType, Class, Args...>
	{

	};

	template < typename T >
	concept has_functor = requires (std::remove_pointer_t<T> v)
	{
		{ &v.operator() };
	};

	
	template <has_functor Function >
	struct Fn_Traits< Function >
		: Fn_Traits < decltype(&std::remove_pointer_t < std::decay_t< Function > >::operator()) >
	{

	};

	template<typename CallBackType>
	concept is_void_Fn = requires
	{
		{has_functor <CallBackType>};
		{ std::is_same_v<void , Fn_Traits<CallBackType>::ReturnType_t > };
	}; 
	template<typename CallBackType>
	concept is_bool_fn = requires
	{
		{has_functor <CallBackType>};
		{ std::is_same_v<bool , Fn_Traits<CallBackType>::ReturnType_t> };
	};



	template<typename CallBackType>
	concept is_empty_void_Fn = requires
	{
		{Fn_Traits<CallBackType>::args_Count == 0};
		{ std::is_same_v< typename Fn_Traits<CallBackType>::ReturnType_t, void> };
	};
	template<typename ... Args>
	constexpr auto make_null_tuple_from_args_v = static_cast<std::tuple<Args...>*>(nullptr);

	template<typename Tuple>
	constexpr auto cast_null_tuple_v = static_cast<Tuple*>(nullptr);


	namespace details
	{
		template<typename T>
		struct is_callable_impl
		{
			template<auto U>     struct Check;
			template<typename>   static std::false_type Test(...);
			template<typename C> static std::true_type  Test(Check<&C::operator()>*);
			constexpr static auto value = std::is_same_v< decltype(Test<T>(nullptr)), std::true_type>;
		};

		template<typename T>
		constexpr auto is_callable_v = std::conditional_t< std::is_class<T>::value, is_callable_impl<T>, std::false_type >::value;
	}
	template<typename T>
	constexpr auto is_callable_v = details::is_callable_v<std::decay_t<T>>;
	namespace details
	{
		//<! Get the index of a type in a tuple
		template < typename SearchType, typename NonTuple >
		struct tuple_to_index;

		template < typename SearchType, typename... OtherTypes >
		struct tuple_to_index < SearchType, std::tuple < SearchType, OtherTypes... > >
		{
			static constexpr auto value = 0;
		};

		template < typename SearchType, typename CurrentType, typename... OtherTypes >
		struct tuple_to_index < SearchType, std::tuple< CurrentType, OtherTypes... > >
		{
			static constexpr auto value = 1U + tuple_to_index < SearchType, std::tuple< OtherTypes... > >::value;
		};

		//<! Templated Function Method
		template < typename Tuple, typename SearchType, size_t Count = 0 >
		consteval int get_index_from_tuple(const Tuple& _tupleToSearch)
		{
			static_assert(Count < std::tuple_size_v< Tuple >);

			// Found first instance of type  
			if constexpr (std::is_same_v < CurrentType, std::tuple_element< Count, Tuple >::type)
				return Count;

			// Go to next element to search for type
			return get_index_from_tuple< Tuple, SearchType, Count + 1 >(_tupleToSearch);
		}
	}

	template < typename SearchType, typename Tuple >
	constexpr auto tuple_to_index_v = details::tuple_to_index < SearchType, Tuple >::value;

}