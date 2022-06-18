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
	template<typename T, typename ...Args>
	constexpr size_t TupleToIndex_v = details::TupleToIndex<T, std::tuple<Args...>>::value;

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

	template<typename Args>
	constexpr auto cast_null_tuple_v = static_cast<Args*>(nullptr);

}