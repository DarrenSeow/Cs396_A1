
namespace Entity
{
	inline Archetype::Archetype& EntityManager::GetOrCreateArchetype(std::span < const Component::ComponentInfo* const> _span) noexcept
	{
		ECS_Tools::Bits Query;
		for (const auto& componentInfo : _span)
		{
			assert(componentInfo->m_uid != ECS_Utility::invalid_component_info_value);
			Query.SetBit(componentInfo->m_uid);
		}

		//make sure entity component exist 

		for (auto& E : m_archetypeBits)
		{
			if (E.Compare(Query))
			{
				const auto index = static_cast<std::size_t>(&E - m_archetypeBits.data());
				return *m_archetypes[index];
			}
		}

		//create archetype...

		m_archetypes.push_back(std::make_unique<Archetype::Archetype>(_span, Query, *this));

		m_archetypeBits.push_back(Query);

		return *m_archetypes.back();
	}
	inline Entity EntityManager::AllocNewEntity(ECS_Utility::EntityIndex _index, Archetype::Archetype& _archetype) noexcept
	{
		assert(m_emptyHead < ECS_Utility::max_num_of_entity);
		auto entityIndex = m_emptyHead;
		auto& record = m_entityRecords[entityIndex];
		m_emptyHead = record.m_index;
		
		record.m_index = _index;
		record.m_archetype = &_archetype;

		return
		{
			.m_index = entityIndex,
			.m_validation = record.m_validation
		};

	}
	inline void EntityManager::SystemDeleteEntity(Entity _entity) noexcept
	{
		auto& entry = m_entityRecords[_entity.m_index];
		++entry.m_validation.m_generation;
		entry.m_validation.m_isDead = false;
		entry.m_index = m_emptyHead;
		m_emptyHead = _entity.m_index;
	}
	inline void EntityManager::SystemDeleteEntity(Entity _entity, Entity& _swappedentity) noexcept
	{
		auto& entry = m_entityRecords[_entity.m_index];
		m_entityRecords[_swappedentity.m_index].m_index = entry.m_index;
		++entry.m_validation.m_generation;
		entry.m_validation.m_isDead = false;
		entry.m_index = m_emptyHead;
		m_emptyHead = _entity.m_index;
	}
	inline EntityManager::EntityManager() noexcept
	{
		//linked list of empty entries
		for (int i = 0; i < ECS_Utility::max_num_of_entity - 2; ++i)
		{
			m_entityRecords[i].m_index = i + 1;
		}
	}
	inline void EntityManager::DeleteEntity(Entity& _entity) noexcept
	{
		assert(_entity.IsDead() == false);
		auto& info = GetEntityDetails(_entity);
		assert(info.m_validation == _entity.m_validation);
		info.m_archetype->DestroyEntity(_entity);
	}
	inline std::vector<Archetype::Archetype*> EntityManager::Search(std::span<const Component::ComponentInfo* const> _types) const noexcept
	{
		ECS_Tools::Bits Query;
		for (const auto& info : _types)
		{
			Query.SetBit(info->m_uid);
		}

		std::vector<Archetype::Archetype*> archetypesFound;
		for (auto& bits : m_archetypeBits)
		{
			if (bits.Compare(Query))
			{
				const auto index = static_cast<std::size_t>(&bits - &m_archetypeBits[0]);
				archetypesFound.push_back(m_archetypes[index].get());
			}
		}
		return archetypesFound;
	}
	inline std::vector<Archetype::Archetype*> EntityManager::Search(const Query::Query& _query) const noexcept
	{
				
		std::vector<Archetype::Archetype*> archetypesFound;
		for (auto& bits : m_archetypeBits)
		{
			if (_query.Compare(bits))
			{
				const auto index = static_cast<std::size_t>(&bits - &m_archetypeBits[0]);
				archetypesFound.push_back(m_archetypes[index].get());
			}
		}
		return archetypesFound;
	}
	inline const EntityRecord& EntityManager::GetEntityDetails(Entity& _ent) noexcept
	{
		EntityRecord& record = m_entityRecords[_ent.m_index];
		assert(_ent.m_validation == record.m_validation);

		return record;
	}
	template<typename... Components>
	inline Archetype::Archetype& EntityManager::GetOrCreateArchetype() noexcept
	{
		
		static_assert(((std::is_same_v<std::decay_t<Components>, Entity> == false)&&...));

		static constexpr auto ComponentList = std::array{ &Component::ComponentInfo_v<Entity>,&Component::ComponentInfo_v<Components>... };

		return GetOrCreateArchetype(ComponentList);
	}

	//template<Tools::is_bool_fn Function>
	template<typename Function>
		requires std::is_same_v<bool, typename Tools::Fn_Traits<Function>::ReturnType_t>&& Tools::has_functor<Function>
	inline void EntityManager::Foreach(const std::vector<Archetype::Archetype*>& _archetypeList, Function&& _func) const noexcept
	{

		using func_traits = Tools::Fn_Traits<Function>;

		for (const auto& archetype : _archetypeList)
		{
			const auto& pool = archetype->m_componentPool;

			auto CachePointers = [&]<typename...Components>(std::tuple<Components...>*) constexpr noexcept
			{
				return std::array
				{
					[&] <typename T>(std::tuple<T>*) constexpr noexcept
					{
						const auto index = pool.FindComponentIndexFromUID(Component::ComponentInfo_v<T>.m_uid);
						if constexpr (std::is_pointer_v<T>) return (index < 0) ? nullptr : pool.m_componentPools[index];
						else								return pool.m_componentPools[index];
					}(Tools::make_null_tuple_from_args_v<Components>)...
				};
			}(Tools::cast_null_tuple_v<func_traits::Args_Tuple>);

			bool skip = false;

			archetype->AccessGuard([&]
				{
					for (auto i = pool.Size(); i; --i)
					{
						if ([&]<typename ... Components>(std::tuple<Components...>*) constexpr noexcept
						{
							return _func([&]<typename T>(std::tuple<T>*) constexpr noexcept ->T
							{
								auto& myPointer = CachePointers[Tools::TupleToIndex_v<T, Components...>];
								if constexpr(std::is_pointer_v<T>)
								{
									if (myPointer == nullptr)
									{
										return reinterpret_cast<T>(nullptr);
									}
								}
								auto p = myPointer;
								myPointer += sizeof(std::decay_t<T>);
								if constexpr (std::is_pointer_v<T>)
								{
									return reinterpret_cast<T>(p);
								}
								else
								{
									return reinterpret_cast<T>(*p);
								}
							}(Tools::make_null_tuple_from_args_v<Components>)...);
						}(Tools::cast_null_tuple_v<func_traits::Args_Tuple>))
						{
							skip = true;
							break;
						}
					}
				});
			if (skip) break;
		}
	}

	//template<Tools::is_void_Fn Function>
	template<typename Function>
		requires  std::is_same_v<void, typename Tools::Fn_Traits<Function>::ReturnType_t>&& Tools::has_functor<Function>
	inline void EntityManager::Foreach(const std::vector<Archetype::Archetype*> _archetypeList, Function&& _func) const noexcept
	{
		using func_traits = Tools::Fn_Traits<Function>;

		for (const auto& archetype : _archetypeList)
		{
			const auto& pool = archetype->m_componentPool;

			auto CachePointers = [&]<typename...Components>(std::tuple<Components...>*) constexpr noexcept
			{
				return std::array
				{
					[&] <typename T>(std::tuple<T>*) constexpr noexcept
					{
						const auto index = pool.FindComponentIndexFromUID(Component::ComponentInfo_v<T>.m_uid);
						if constexpr (std::is_pointer_v<T>) return (index < 0) ? nullptr : pool.m_componentPools[index];
						else								return pool.m_componentPools[index];
					}(Tools::make_null_tuple_from_args_v<Components>)...
				};
			}(Tools::cast_null_tuple_v<func_traits::Args_Tuple>);

			bool skip = false;

			archetype->AccessGuard([&]
				{
					for (ECS_Utility::EntityIndex i = pool.Size(); i; --i)
					{
						[&]<typename ... Components>(std::tuple<Components...>*) constexpr noexcept
						{
							return _func([&]<typename T>(std::tuple<T>*) constexpr noexcept ->T
							{
								auto& myPointer = CachePointers[Tools::TupleToIndex_v<T, Components...>];
								if constexpr(std::is_pointer_v<T>)
								{
									if (myPointer == nullptr)
									{
										return reinterpret_cast<T>(nullptr);
									}
								}
								auto p = myPointer;
								myPointer += sizeof(std::decay_t<T>);
								if constexpr (std::is_pointer_v<T>)
								{
									return reinterpret_cast<T>(p);
								}
								else
								{
									return reinterpret_cast<T>(*p);
								}
							}(Tools::make_null_tuple_from_args_v<Components>)...);
						}(Tools::cast_null_tuple_v<func_traits::Args_Tuple>);

					}
				});
			
		}
	}

	//template<Tools::is_void_Fn CallBackType>
	template<typename CallBackType>
		requires  std::is_same_v<void, typename Tools::Fn_Traits<CallBackType>::ReturnType_t>
	inline Entity EntityManager::CreateEntity(CallBackType&& _function) noexcept
	{
		using func_traits = Tools::Fn_Traits<CallBackType>;

		return[&]<typename ... Components>(std::tuple<Components...>*) constexpr noexcept
		{
			auto& archetype = GetOrCreateArchetype<Components...>();

			assert(archetype.m_bits.GetBit(Component::ComponentInfo_v<Components>.m_uid) && ...);
			//add entity to its respective archetype
			const auto EntityIndexInPool = archetype.AppendEntity();
			//add entity to the global pool

			const auto entity = AllocNewEntity(EntityIndexInPool, archetype);
			archetype.GetComponent<Entity>(EntityIndexInPool) = entity;

			if constexpr (false == std::is_same_v<ECS_Utility::EmptyLambda, CallBackType>)
				_function(archetype.GetComponent<std::remove_reference_t<Components>>(EntityIndexInPool)...);
			return entity;
		}(Tools::cast_null_tuple_v<func_traits::Args_Tuple>);
	}
	template<typename ...Components>
	inline Entity EntityManager::CreateEntity() noexcept
	{
		auto & archetype = GetOrCreateArchetype<Components...>();

		assert(archetype.m_bits.GetBit(Component::ComponentInfo_v<Components>.m_uid) && ...);
		//add entity to its respective archetype
		const auto EntityIndexInPool = archetype.AppendEntity();
		//add entity to the global pool

		const auto entity = AllocNewEntity(EntityIndexInPool, archetype);
		return entity;
	}
	//template<Tools::is_void_Fn CallBackType>
	template<typename CallBackType>
		requires  std::is_same_v<void, typename Tools::Fn_Traits<CallBackType>::ReturnType_t>
	inline bool EntityManager::FindEntity(Entity _ent, CallBackType&& Function) const noexcept
	{
		if (_ent.IsDead()) return false;
		

		auto& record = m_entityRecords[_ent.m_index];

		if (record.m_validation == _ent.m_validation)
		{
			if constexpr (!std::is_same_v<CallBackType, ECS_Utility::EmptyLambda>)
			{
				[&] <typename... Components>(std::tuple<Components...>*) constexpr noexcept
				{
					Function(record.m_archetype->GetComponent<std::remove_reference_t<Components>>(record.m_index)...);
				}(Tools::cast_null_tuple_v<Tools::Fn_Traits<CallBackType>::Args_Tuple>);
			}
			return true;
		}
		return false;
		
	}
	template<typename ...Components>
	inline std::vector<Archetype::Archetype*> EntityManager::Search() const noexcept
	{
	
	//	static_assert(((std::is_same_v<<Components>, Entity>)&&...));
		if constexpr (((std::is_same_v<Components, Entity>)&&...))
		{
			static constexpr auto ComponentList = std::array{ &Component::ComponentInfo_v<Components>... };
			return Search(ComponentList);
		}
		
		static constexpr auto ComponentList = std::array{ &Component::ComponentInfo_v<Entity>, &Component::ComponentInfo_v<Components>... };
		return Search(ComponentList);
		
	}
}