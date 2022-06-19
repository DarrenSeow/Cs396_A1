#pragma once

/******************************************************************************
filename: ComponentInfo.h
author: Seow Jun Hao Darren seow.j@digipen.edu
Project: Cs396_A1 
Description:
Contains the definition of ComponentInfo and type erasure of it
******************************************************************************/
namespace Component
{
	class ComponentInfo final
	{
		using constructorFn		= void(*) (std::byte* _src) noexcept;
		using destructorFn		= void(*) (std::byte* _src) noexcept;
		using copyConstructorFn = void(*) (std::byte* _src) noexcept;
		using moveFn			= void(*) (std::byte* _src,std::byte* _dst) noexcept;
	public:
		mutable ECS_Utility::ComponentUID	m_uid; //unique id of component
		size_t							m_size;//size of component
		constructorFn					m_construct;
		destructorFn					m_destruct;
		moveFn							m_move;
	};

	namespace Details
	{
		template<typename T>
		consteval auto CreateComponentInfo() noexcept
		{
			return ComponentInfo
			{
				.m_uid			= ECS_Utility::invalid_component_info_value,
				.m_size			= sizeof(T),
				.m_construct	= std::is_trivially_constructible_v<T> ? nullptr :
																	[](std::byte* _src) noexcept
																	{
																		new (_src) T;
																	} ,
				.m_destruct		= std::is_trivially_destructible_v<T> ? nullptr :
																	[](std::byte* _src) noexcept
																	{
																		std::destroy_at(reinterpret_cast<T*>(_src));
																	} ,
				.m_move			= std::is_trivially_move_assignable_v<T> ? nullptr:
																		[](std::byte* _src,std::byte* _dst) noexcept 
																	{
																		*reinterpret_cast<T*>(_dst) = std::move(*reinterpret_cast<T*>(_src));
																	}


			};
		}

		template<typename T>
		static constexpr auto CreateComponentInfo_v = CreateComponentInfo<T>();
	}
	
	template<typename T>
	constexpr auto& ComponentInfo_v = Details::CreateComponentInfo_v<std::decay_t<T>>;

	
}