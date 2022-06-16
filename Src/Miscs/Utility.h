 #pragma once
namespace ECS_Utility
{
	using EntityID		= std::uint64_t;
	using EntityIndex	= std::uint32_t;
	using ValidEntityID = std::uint32_t;
	
	using ComponentUID	= std::uint64_t;
	using ComponentSize = std::uint64_t;
	
	using EmptyLambda = decltype([]() {});


	constexpr auto virtual_page_size = 4096;

	constexpr auto invalid_component_info_value = std::numeric_limits<ComponentUID>::max();

	constexpr auto max_num_of_entity = 10000;

	constexpr auto max_num_entity_per_page = 1000;
	
	constexpr auto max_num_of_component = 128;

	constexpr auto max_num_of_component_per_entity = 32;
}