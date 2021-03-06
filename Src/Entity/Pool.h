#pragma once
/******************************************************************************
filename: Pool.h
author: Seow Jun Hao Darren seow.j@digipen.edu
Project: Cs396_A1
Description:
Contains the declaration of Pool where archetype will keep this to handle
its component data
******************************************************************************/
namespace Archetype
{
	class Pool final
	{
		//size of the pool aka the number of entity in the pool
		ECS_Utility::EntityIndex m_size{ 0 };
		std::span<const Component::ComponentInfo* const> m_infos{};

	public:
		std::array<std::byte*, ECS_Utility::max_num_of_component_per_entity> m_componentPools{};
		Pool() noexcept = default;
		Pool(std::span<const Component::ComponentInfo* const> _infos);
		Pool(Pool&) = delete;
		~Pool() noexcept;

		ECS_Utility::EntityIndex Append() noexcept;
		void Clear() noexcept;
		void Delete(ECS_Utility::EntityIndex _index) noexcept;
		constexpr ECS_Utility::EntityIndex Size()  const noexcept;
		void Initialize(std::span<const Component::ComponentInfo* const> _span) noexcept;
		constexpr size_t GetPageFromIndex(const Component::ComponentInfo& _info, ECS_Utility::EntityIndex index) noexcept;

		constexpr int FindComponentIndexFromUID(const ECS_Utility::ComponentUID _UID) const noexcept;

		template<typename TComponent>
		requires std::is_same_v<TComponent, std::decay_t<TComponent>>
		TComponent& GetComponent(const ECS_Utility::EntityIndex _index) noexcept;

	};
}
