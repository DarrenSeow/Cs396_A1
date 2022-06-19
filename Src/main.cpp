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

struct TestSystem : System::System
{
    void operator()(Position& _pos/*, Scale* _scale*/)
    {
        _pos.pos.x += 10;
        std::cout << "Hello" << _pos.pos.x << std::endl;

        //_scale->m_sx += 10;
        //std::cout << "scale" << _scale->m_sx << std::endl;
    }
    //void Update() noexcept
    //{
    //    std::cout << "system check" << std::endl;
    //}
};
int main()
{
    ECS::ECSManager ecsMgr;
	std::cout << Component::ComponentInfo_v<Rotation>.m_uid << std::endl;

	
    //ecsMgr.RegisterComponents<Entity::Entity>();
    std::cout << Component::ComponentInfo_v<Entity::Entity>.m_uid << std::endl;
    ecsMgr.RegisterComponents<Rotation, Scale, Position>();

	std::cout << Component::ComponentInfo_v<Rotation>.m_uid << std::endl;
	std::cout << Component::ComponentInfo_v<Position>.m_uid << std::endl;
	
	std::cout << Component::ComponentInfo_v<Scale>.m_uid << std::endl;
    
    ecsMgr.RegisterSystems<TestSystem>();
    Query::Query query;
    ECS_Tools::Bits bit;
    bit.SetBit(Component::ComponentInfo_v<Position>.m_uid);
    //auto ent1 = ecsMgr.CreateEntity<Position, Scale>();
    ecsMgr.CreateEntity<Position>();
    //query.AddQueryFromFunction([](Position&,Scale*) {});
    std::tuple<Query::Must<Position>> testquery;
    
    query.AddQueryFromTuple(&testquery);
    Entity::EntityManager entityMgr;
    //ecsMgr.FindEntity(ent1, [](Position& _pos) {std::cout << "template" << std::endl; });
    ecsMgr.Foreach(ecsMgr.Search(query),[](Position& _pos) {std::cout << "hello world" << _pos.pos.x << std::endl; });
    ecsMgr.Run();
}