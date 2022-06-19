#pragma once
/******************************************************************************
filename: ECSManager.h
author: Seow Jun Hao Darren seow.j@digipen.edu
Project: Cs396_A1
Description:
Contains the declaration of ECSManager that manages the 3 other managers
******************************************************************************/
namespace ECS
{
	class ECSManager
	{
		std::unique_ptr<Entity::EntityManager> m_entityMgr;
		std::unique_ptr<Component::ComponentManager> m_componentMgr;
		std::unique_ptr<System::SystemManager> m_systemMgr;
		//System::SystemManager m_systemMgr;
	public:
		ECSManager() noexcept;

		ECSManager(ECSManager&) = delete;
		ECSManager(ECSManager&&) = delete;

		ECSManager& operator=(ECSManager&) = delete;
		ECSManager& operator=(ECSManager&&) = delete;
		

		void DeleteEntity(Entity::Entity& _entity) noexcept;

		//template<Tools::is_bool_fn Function>
		//template<typename Function>
		template<typename Function>
			requires (Tools::has_functor<Function>&& std::is_same_v<bool, typename Tools::Fn_Traits<Function>::ReturnType_t>)
		void Foreach(const std::vector<Archetype::Archetype*>& _archetypeList, Function&& _func) const noexcept;

		//template<Tools::is_void_Fn Function>
		//template<typename Function>
		template<typename Function>
			requires (Tools::has_functor<Function>&& std::is_same_v<void, typename Tools::Fn_Traits<Function>::ReturnType_t>)
		void Foreach(const std::vector<Archetype::Archetype*> _list, Function&& _func) const noexcept;
		//template<Tools::is_void_Fn CallBackType>
		template<typename CallBackType>
			requires  std::is_same_v<void, typename Tools::Fn_Traits<CallBackType>::ReturnType_t>
		Entity::Entity CreateEntity(CallBackType&& _function) noexcept;
		template<typename... Components>
		Entity::Entity CreateEntity() noexcept;

		template<typename ... Components>
		void RegisterComponents() noexcept;

		template<typename CallBackType>
			requires  std::is_same_v<void, typename Tools::Fn_Traits<CallBackType>::ReturnType_t>
		bool FindEntity(Entity::Entity _ent, CallBackType&& Function) const noexcept;

		std::vector<Archetype::Archetype*> Search(std::span < const Component::ComponentInfo* const> _types) const noexcept;

		template<typename ... Components>
		std::vector<Archetype::Archetype*> Search() const noexcept;

		std::vector<Archetype::Archetype*> Search(const Query::Query& _query) const noexcept;

		template<typename ... T_Systems>
		requires (std::derived_from<T_Systems,System::System> && ...)
		void RegisterSystems() noexcept;

		void Run() noexcept;
	};
}