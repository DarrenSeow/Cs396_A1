
#pragma once
/******************************************************************************
filename: Pool.h
author: Seow Jun Hao Darren seow.j@digipen.edu
Project: Cs396_A1
Description:
Contains the defintion of Pool that contains the components data
******************************************************************************/
namespace Archetype
{
    inline Pool::Pool(std::span<const Component::ComponentInfo* const> _infos)
    {
        Initialize(_infos);
    }
    inline Pool::~Pool() noexcept
    {
        if (!m_componentPools.front()) return;
        Clear();
        for (auto* ptr : m_componentPools)
        {
            VirtualFree(ptr, 0, MEM_RELEASE);
        }
    }
    inline ECS_Utility::EntityIndex Pool::Append() noexcept
    {

        assert(m_size < ECS_Utility::max_num_of_entity - 1);

        for (int i = 0; i < m_infos.size(); i++)
        {
            const auto& info = *m_infos[i];
            const auto nextPage = GetPageFromIndex(info, m_size + 1);

            if (GetPageFromIndex(info, m_size) != nextPage)
            {
                auto pagePtr = m_componentPools[i] + nextPage * ECS_Utility::virtual_page_size;
                auto allocatedLocation = reinterpret_cast<std::byte*>(VirtualAlloc(pagePtr, ECS_Utility::virtual_page_size, MEM_COMMIT, PAGE_READWRITE));
                assert(pagePtr == allocatedLocation);
            }
            if (info.m_construct)
            {
                info.m_construct(m_componentPools[i] + m_size * info.m_size);
            }
        }
        return m_size++;
    }
    inline void Pool::Clear() noexcept
    {
        while (m_size)
        {
            Delete(m_size-1);
        }
    }
    inline void Pool::Delete(ECS_Utility::EntityIndex _index) noexcept
    {
        assert(_index < ECS_Utility::max_num_of_entity - 1);

        if ( _index== --m_size)
        {
            //last entity in the pool
            for (size_t i = 0; i < m_infos.size(); i++)
            {
                const auto& info = *m_infos[i];
                const auto nextPage = GetPageFromIndex(info, _index + 1);
                if (info.m_destruct)
                {
                    info.m_destruct(m_componentPools[i] + m_size * info.m_size);
                }
                if (GetPageFromIndex(info, _index) != nextPage)
                {
                    auto pagePtr = m_componentPools[i] + nextPage * ECS_Utility::virtual_page_size;
                    auto free = VirtualFree(pagePtr, ECS_Utility::virtual_page_size, MEM_DECOMMIT);
                    assert(free);
                }

            }
        }
        else
        {
            //entity in the middle of nowhere
            for (size_t i = 0; i < m_infos.size(); i++)
            {
                const auto& info = *m_infos[i];
                const auto nextPage = GetPageFromIndex(info, m_size + 1);


                if (info.m_move)
                {
                    info.m_move(m_componentPools[i] + m_size * info.m_size, m_componentPools[i] + _index * info.m_size);
                }
                else
                {
                    if (info.m_destruct)
                    {
                        info.m_destruct(m_componentPools[i] + _index * info.m_size);
                    }
                    std::memcpy(m_componentPools[i] + _index * info.m_size, m_componentPools[i] + m_size * info.m_size, info.m_size);
                   
                }
                
                if (GetPageFromIndex(info, m_size) != nextPage)
                {
                    auto free = VirtualFree(m_componentPools[i] + nextPage * ECS_Utility::virtual_page_size,
                        ECS_Utility::virtual_page_size, MEM_DECOMMIT);
                    assert(free);
                }
                
            }
        }


    }
    inline constexpr ECS_Utility::EntityIndex Pool::Size()  const noexcept
    {
        return m_size;
    }
    inline void Pool::Initialize(std::span<const Component::ComponentInfo* const> _span) noexcept
    {
        //assert if the size contains too much for the archetypePool
        //due to the max size limitation
        assert(_span.size() <= ECS_Utility::max_num_of_component_per_entity);
        m_infos = _span;
        //initialize the pages for all components in the pool
        for (size_t i = 0; i < m_infos.size(); i++)
        {
            assert(m_infos[i]->m_size <= ECS_Utility::virtual_page_size);
            auto numOfPages = GetPageFromIndex(*m_infos[i], ECS_Utility::virtual_page_size + 1);
            m_componentPools[i] = reinterpret_cast<std::byte*>(VirtualAlloc(nullptr, numOfPages * ECS_Utility::max_num_entity_per_page, MEM_RESERVE, PAGE_NOACCESS));
            assert(m_componentPools[i]);
        }
    }
    inline constexpr size_t Pool::GetPageFromIndex(const Component::ComponentInfo& _info, ECS_Utility::EntityIndex _index) noexcept
    {
        return (_index * _info.m_size - 1) / ECS_Utility::virtual_page_size;
    }

    inline constexpr int Pool::FindComponentIndexFromUID(const ECS_Utility::ComponentUID _UID) const noexcept
    {
        for (int i = 0; i < m_infos.size(); ++i)
        {
            if (m_infos[i]->m_uid == _UID)
            {
                return i;
            }
        }
        return -1;
    }

    template<typename TComponent>
    requires std::is_same_v<TComponent,std::decay_t<TComponent>>
    inline TComponent& Pool::GetComponent(const ECS_Utility::EntityIndex _index) noexcept 
    {
        
        const auto componentIndex = FindComponentIndexFromUID(Component::ComponentInfo_v<TComponent>.m_uid);
        assert(componentIndex >= 0);
        return *reinterpret_cast<TComponent*>(m_componentPools[componentIndex] + _index * sizeof(TComponent));
    }
}
