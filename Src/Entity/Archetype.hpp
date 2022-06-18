#include "Archetype.h"
namespace Archetype
{
	Archetype::Archetype(const std::span<const Component::ComponentInfo* const >& _infos,	const ECS_Tools::Bits& _bits,
		 Entity::EntityManager& _entityMgr) noexcept
		:	
		m_entityMgr{_entityMgr},
		m_bits{_bits},
		//m_infoData{},
		m_componentPool{_infos}

	{

	}
	ECS_Utility::EntityIndex Archetype::AppendEntity() noexcept
	{
		return m_componentPool.Append();
	}

	inline void Archetype::UpdateStructuralChange() noexcept
	{
		if (!m_entityToDelete.empty())
		{
			for (auto& entity : m_entityToDelete)
			{
				auto& entityDetails = m_entityMgr.GetEntityDetails(entity);

				assert(entityDetails.m_archetype == this);

				m_componentPool.Delete(entityDetails.m_index);
				if (entityDetails.m_index != m_componentPool.Size())
				{
					m_entityMgr.SystemDeleteEntity(entity, m_componentPool.GetComponent<Entity::Entity>(entityDetails.m_index));
				}
				else
				{
					m_entityMgr.SystemDeleteEntity(entity);
				}
			}
			m_entityToDelete.clear();
		}
	}

	inline void Archetype::DestroyEntity(Entity::Entity& _entity) noexcept
	{
		assert(_entity.IsAlive() == false);
		_entity.m_validation.m_isAlive = m_entityMgr.m_entityRecords[_entity.m_index].m_validation.m_isAlive = true;
		m_entityToDelete.push_back(_entity);

		if (0 == m_processRef)
		{
			UpdateStructuralChange();
		}
	}

	template<typename Component>
	inline Component& Archetype::GetComponent(const ECS_Utility::EntityIndex _index) noexcept
	{
		return m_componentPool.GetComponent<Component>(_index);
	}
	template<Tools::is_empty_void_Fn Function>
	inline void Archetype::AccessGuard(Function&& _func) noexcept
	{
		++m_processRef;
		_func;
		if (--m_processRef == 0) UpdateStructuralChange();
	}
}