#include "Miscs/Includes.h"
struct Vector2D
{
    float x, y;

    Vector2D(const float _x = 0.f, const float _y = 0.f)
        : x{ _x }, y{ _y }
    { }
};

struct Position
{
    Vector2D pos{ 5.f, 5.f };
};

struct Scale
{
    float m_sx;
    float m_sy;
    int* ptr;

    Scale()
        : m_sx{ 1.f },
        m_sy{ 1.f },
        ptr{ new int }
    {
        std::cout << "Called Scale ctor" << std::endl;
    }

    Scale& operator=(const Scale&& _rhs) noexcept
    {
        m_sx = _rhs.m_sx;
        m_sy = _rhs.m_sy;

        delete ptr;
        ptr = _rhs.ptr;

        std::cout << "Called Scale move" << std::endl;
        return *this;
    }

    ~Scale()
    {
        m_sx = m_sy = 0.f;
        delete ptr;
        ptr = nullptr;

        std::cout << "Called Scale dtor" << std::endl;
    }
};

struct Rotation
{
    float m_rz = 45.f;
};
void fn() {};
int main()
{
	std::cout << Component::ComponentInfo_v<Rotation>.m_uid << std::endl;

	std::unique_ptr<Component::ComponentManager> m_compMgr = std::make_unique<Component::ComponentManager>();

	m_compMgr->RegisterComponents<Rotation, Scale, Position>();
	std::cout << Component::ComponentInfo_v<Rotation>.m_uid << std::endl;
	
	std::cout << Component::ComponentInfo_v<Scale>.m_uid << std::endl;
    
    {
        std::cout << "\033[1m\033[33m" << "\n----- START TEST 02 -----\n" << "\033[0m\033[37m" << std::endl;
        using info_type = const Component::ComponentInfo* const;
        constexpr auto info_size_v = 3;

        std::array< info_type, info_size_v> arr
        {
            &Component::ComponentInfo_v< Rotation >,
            &Component::ComponentInfo_v< Scale >,
            &Component::ComponentInfo_v< Position >
        };

        Archetype::Pool poolInst{ std::span< info_type >{ arr.data(), arr.size()} };
        

        const auto index = poolInst.Append();
        const auto index2 = poolInst.Append();
        const auto index3 = poolInst.Append();

        std::cout << "Finding Scale Component in pool ... " << std::endl;
        const std::int32_t findCompIndex = poolInst.FindComponentIndexFromUID(arr[1]->m_uid);

        findCompIndex > 0 ?
            std::cout << "Found at " << findCompIndex << std::endl :
            std::cout << "Component not found." << std::endl;

       // auto& comp1 = poolInst.GetComponent< Vector2D >(index); // Test assert
       auto& comp = poolInst.GetComponent< Rotation >(index);

        std::cout
            << "Rotation uid: "
            << Component::ComponentInfo_v < decltype(comp) >.m_uid
            << std::endl; // shld be 1 (order of registration)

        std::cout << "Rotation along z-axis: " << comp.m_rz << std::endl;
        
        poolInst.Delete(index);
        std::cout << "\033[1m\033[33m" << "\n----- END TEST -----\n" << "\033[0m\033[37m" << std::endl;

        ECS_Tools::Bits bit;
        bit.AddFromComponents<Scale>();

        Archetype::Archetype archetype;
        
        archetype.CreateEntity([]() {});
    }
}