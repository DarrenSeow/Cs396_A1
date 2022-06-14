#pragma once

namespace Archetype
{
	class Pool final
	{
		//size of the pool aka the number of entity in the pool
		ECS_Utility::EntityIndex m_size{ 0 };
		std::span<const Component::ComponentInfo* const> m_infos{};
		std::array<std::byte*, ECS_Utility::max_num_of_component_per_entity> m_componentPools{};
	public:
		Pool() noexcept = default;
		Pool(Pool&) = delete;
		~Pool() noexcept;

		ECS_Utility::EntityIndex Append() noexcept;
		void Clear() noexcept;
		void Delete(ECS_Utility::EntityIndex _index) noexcept;

		void Initialize(std::span<const Component::ComponentInfo* const> _span) noexcept;
		constexpr size_t GetPageFromIndex(const Component::ComponentInfo& _info, ECS_Utility::EntityIndex index) noexcept;

		constexpr int FindComponentIndexFromUID(const ECS_Utility::ComponentUID _UID) const noexcept;

		template<typename TComponent>
		requires std::is_same_v<TComponent, std::decay_t<TComponent>>
		TComponent& GetComponent(const ECS_Utility::EntityIndex _index) noexcept;

	};
}

#include "Pool.hpp"