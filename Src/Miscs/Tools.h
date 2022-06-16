#pragma once
namespace Tools
{
	namespace details
	{
		template<typename T, typename ... Args>
		struct TupleToIndex;

		template<typename T, typename ... Args>
		struct TupleToIndex<T, std::tuple<T,Args...>>
		{
			static constexpr size_t value = 0;
		};

		template<typename T,typename U, typename ... Args>
		struct TupleToIndex<T,std::tuple<U,Args...>>
		{
			static constexpr size_t value = 1 + TupleToIndex<T, Args...> ::value;
		};


		template<typename ReturnType,typename Class, typename ... Args>
		struct Fn_Traits_Info
		{
			using ReturnType_t = ReturnType;
			using Class_t = Class;
			using Parameters_t = std::tuple<Args...>;

			static constexpr auto args_Count = sizeof...(Args);
		};

		
	}
	template<typename T,typename ...Args>
	constexpr size_t TupleToIndex_v = TupleToIndex<T, std::tuple<Args...>>::value;

	template<typename NonFunction>
	struct Fn_Traits;

	//normal function specialization
	template <typename ReturnType, typename ...Args>
	struct Fn_Traits<ReturnType(Args...)> :
		Fn_Traits_Info<ReturnType, void, Args...>
	{

	};


	// function pointer specialization
	template<typename ReturnType, typename ...Args>
	struct Fn_Traits<ReturnType(*)(Args...)> :
		Fn_Traits_Info<ReturnType, void, Args...>
	{

	};
	//class function specialization
	template<typename ReturnType, typename Class, typename ...Args>
	struct Fn_Traits<ReturnType(Class::*)(Args...)> :
		Fn_Traits_Info<ReturnType, Class, Args...>
	{

	};

	//const class function/lambda specialization
	template<typename ReturnType, typename Class, typename ...Args>
	struct Fn_Traits<ReturnType(Class::*)(Args...) const> :
		Fn_Traits_Info<ReturnType, Class, Args...>
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
}