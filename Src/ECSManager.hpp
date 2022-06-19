
#pragma once

/******************************************************************************
filename: ECSManager.hpp
author: Seow Jun Hao Darren seow.j@digipen.edu
Project: Cs396_A1
Description:
Contains the defintion of ECSManager that manages the 3 other managers
******************************************************************************/
namespace ECS
{
	ECSManager::ECSManager() noexcept :
		m_entityMgr{std::make_unique<Entity::EntityManager>()},
		m_componentMgr{std::make_unique<Component::ComponentManager>()},
		m_systemMgr{std::make_unique<System::SystemManager>()}
	{
		m_componentMgr->RegisterComponent<Entity::Entity>();
	}
	inline void ECSManager::DeleteEntity(Entity::Entity& _entity) noexcept
	{
		m_entityMgr->DeleteEntity(_entity);
	}
	//template<typename Function>
	template<typename Function>
		requires (Tools::has_functor<Function>&& std::is_same_v<bool, typename Tools::Fn_Traits<Function>::ReturnType_t>)
	void ECSManager::Foreach(const std::vector<Archetype::Archetype*>& _archetypeList, Function&& _func) const noexcept
	{
		m_entityMgr->Foreach(_archetypeList, _func);
	}
	//template<typename Function>
	template<typename Function>
		requires (Tools::has_functor<Function>&& std::is_same_v<void, typename Tools::Fn_Traits<Function>::ReturnType_t>)
	void ECSManager::Foreach(const std::vector<Archetype::Archetype*> _list, Function&& _func) const noexcept
	{
		m_entityMgr->Foreach(_list, std::move(_func));
	}
	template<typename CallBackType>
		requires  std::is_same_v<void, typename Tools::Fn_Traits<CallBackType>::ReturnType_t>
	Entity::Entity ECSManager::CreateEntity(CallBackType&& _function) noexcept 
	{
		return m_entityMgr->CreateEntity(_function);
	}
	template<typename... Components>
	Entity::Entity ECSManager::CreateEntity() noexcept
	{
		return m_entityMgr->CreateEntity<Components...>();
	}
	template<typename CallBackType>
		requires  std::is_same_v<void, typename Tools::Fn_Traits<CallBackType>::ReturnType_t>
	bool ECSManager::FindEntity(Entity::Entity _ent, CallBackType&& Function) const noexcept
	{
		return m_entityMgr->FindEntity(_ent, Function);
	}
	std::vector<Archetype::Archetype*> ECSManager::Search(std::span < const Component::ComponentInfo* const> _types) const noexcept
	{
		return m_entityMgr->Search(_types);
	}

	template<typename ... Components>
	std::vector<Archetype::Archetype*> ECSManager::Search() const noexcept
	{
		return m_entityMgr->Search<Components...>();
	}

	std::vector<Archetype::Archetype*> ECSManager::Search(const Query::Query& _query) const noexcept
	{
		return m_entityMgr->Search(_query);
	}
	inline void ECSManager::Run() noexcept
	{
		m_systemMgr->Run();
	}
	template<typename ...Components>
	inline void ECSManager::RegisterComponents() noexcept
	{
		m_componentMgr->RegisterComponents<Components...>();
	}
	template<typename ... T_Systems>
		requires (std::derived_from<T_Systems, System::System> && ...)
	inline void ECSManager::RegisterSystems() noexcept
	{
		(m_systemMgr->RegisterSystem<T_Systems>(*this),...);
	}
}