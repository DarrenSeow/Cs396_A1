namespace Archetype
{
	Archetype::Archetype(const std::span<const Component::ComponentInfo* const >& _infos,	const ECS_Tools::Bits& _bits,
		 Entity::EntityManager& _entityMgr) noexcept
		:	
		m_entityMgr{_entityMgr},
		m_bits{_bits},
		m_infoData{},
		m_componentPool{_infos}

	{

	}
	ECS_Utility::EntityIndex Archetype::AppendEntity() noexcept
	{
		return m_componentPool.Append();
	}

	template<typename Component>
	inline Component& Archetype::GetComponent(const ECS_Utility::EntityIndex _index) noexcept
	{
		return m_componentPool.GetComponent<Component>(_index);
	}
}