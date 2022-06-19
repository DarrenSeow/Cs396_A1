#include "Miscs/Includes.h"

#include "GL/freeglut.h"
#include <random>
#include "Math/Vector2D.h"
#define GLUT_STATIC_LIB
using entity = Entity::Entity;
static struct game
{
    std::unique_ptr<ECS::ECSManager> m_ecsMgr = std::make_unique<ECS::ECSManager>();
    int m_w = 1024;
    int m_h = 800;
}s_Game;


struct position
{
    Vector2D m_value;

};
struct velocity
{
    Vector2D m_value;
};
struct timer
{
    float m_value{};
};
struct bullet
{
    Entity::Entity m_shipOwner;
};

struct update_movement : System::System
{
    void operator()(position& Position, velocity& velocity) const
    {
        Position.m_value.x += velocity.m_value.x;
        Position.m_value.y += velocity.m_value.y;

        if (Position.m_value.x < 0)
        {
            Position.m_value.x = 0;
            velocity.m_value.x = -velocity.m_value.x;
        }
        else if (Position.m_value.x >= s_Game.m_w)
        {
            Position.m_value.x = s_Game.m_w - 1;
            velocity.m_value.x = -velocity.m_value.x;
        }
        if (Position.m_value.y < 0)
        {
            Position.m_value.y = 0;
            velocity.m_value.y = -velocity.m_value.y;
        }
        else if (Position.m_value.y >= s_Game.m_h)
        {
            Position.m_value.y = s_Game.m_h - 1;
            velocity.m_value.y = -velocity.m_value.y;
        }
    }
};
struct bullet_logic :System::System
{
    void operator()(entity& Entity, position& Position, timer& Timer, bullet& Bullet) const
    {
        if (Entity.IsDead()) return;
        
        Timer.m_value -= 0.01f;
        if (Timer.m_value <= 0)
        {
            m_ecsMgr.DeleteEntity(Entity);
            return;
        }
        Query::Query _Query;
        _Query.m_must.AddFromComponents<position>();
        m_ecsMgr.Foreach(m_ecsMgr.Search(_Query), [&](entity& E, position& Pos) ->bool
            {
                assert(Entity.IsDead() == false);

                // Our we checking against my self?
                if (&Entity == &E) return false;

                // Is this bullet or ship already dead?
                if (E.IsDead()) return false;

                // Are we colliding with our own ship?
                // If so lets just continue
                if (Bullet.m_shipOwner.m_uid == E.m_uid) return false;

                constexpr auto distance_v = 3;
                
                if ((Pos.m_value - Position.m_value) < distance_v * distance_v)
                {
                    m_ecsMgr.DeleteEntity(Entity);
                    m_ecsMgr.DeleteEntity(E);
                    return true;
                }

                return false;
            });        
    }
};
struct space_ship_logic : System::System
{
    using query = std::tuple
    <
        Query::NoneOf<bullet>
    >;
    void operator()(entity& Entity, position& Position, timer& Time)const
    {
        if (Time.m_value > 0)
        {
            Time.m_value -= 0.01f;
            return;
        }

        Query::Query _Query;
        _Query.m_noneOf.AddFromComponents<bullet>();
        m_ecsMgr.Foreach(m_ecsMgr.Search(_Query), [&](position& pos) ->bool
            {
                if (&pos == &Position) return false;

                auto Direction = pos.m_value - Position.m_value;
                const auto DistanceSquare = MagnitudeSquared(Direction);

                constexpr auto min_distance_v = 30;

                if (DistanceSquare < min_distance_v * min_distance_v)
                {
                    Time.m_value = 8;
                    m_ecsMgr.CreateEntity([&](position&, velocity& vel, bullet& Bullet, timer& Timer)
                        {
                            Direction /= std::sqrt(DistanceSquare);
                            vel.m_value = Direction * 2.0f;
                            pos.m_value = Position.m_value + vel.m_value;

                            Bullet.m_shipOwner = Entity;
                            Timer.m_value = 10;

                        });
                    return true;
                }
                return false;
            });
    }
};

struct render_bullets : System::System
{
    using query = std::tuple
    <
    Query::Must<bullet>
    >;

    void operator()(position& Position, velocity& Velocity) const
    {
        constexpr auto SizeX = 1;
        constexpr auto SizeY = SizeX * 3;
        glBegin(GL_TRIANGLES);
        glColor3f(1.0f, .5f, 0.0f);
        glVertex2i(Position.m_value.x + Velocity.m_value.x * SizeY, Position.m_value.y + Velocity.m_value.y * SizeY);
        glVertex2i(Position.m_value.x + Velocity.m_value.y * SizeX, Position.m_value.y - Velocity.m_value.x * SizeX);
        glVertex2i(Position.m_value.x - Velocity.m_value.y * SizeX, Position.m_value.y + Velocity.m_value.x * SizeX);
        glEnd();
    }
};

struct render_ships : System::System
{
    using query = std::tuple<
        Query::NoneOf<bullet>
    >;

    void operator()(position& Position, timer& Timer) const
    {
        const auto Size = 3;
        glBegin(GL_QUADS);
        if (Timer.m_value > 0) glColor3f(1.0, 0.5, 1.0);
        else                   glColor3f(0.5, 1.0, 0.5);
        glVertex2i(Position.m_value.x - Size, Position.m_value.y - Size);
        glVertex2i(Position.m_value.x - Size, Position.m_value.y + Size);
        glVertex2i(Position.m_value.x + Size, Position.m_value.y + Size);
        glVertex2i(Position.m_value.x + Size, Position.m_value.y - Size);
        glEnd();
    }
};

struct page_flip : System::System
{
    __inline void Update() noexcept
    {
        glutSwapBuffers();
        glClear(GL_COLOR_BUFFER_BIT);
    }
};
void InitializeGame() noexcept
{
    std::srand(100);

    s_Game.m_ecsMgr->RegisterComponents
        <position, velocity, timer, bullet>();

   s_Game.m_ecsMgr->RegisterSystems
       <update_movement, space_ship_logic, bullet_logic, render_ships, render_bullets, page_flip>();
   // s_Game.m_ecsMgr->RegisterSystems<page_flip>();
    for (size_t i = 0; i < 1000; i++)
    {
        s_Game.m_ecsMgr->CreateEntity([&](position& Position, velocity& Velocity, timer& Timer)
            {
                Position.m_value.x = std::rand() % s_Game.m_w;
                Position.m_value.y = std::rand() % s_Game.m_h;

                Velocity.m_value.x = (std::rand() / (float)RAND_MAX) - 0.5f;
                Velocity.m_value.y = (std::rand() / (float)RAND_MAX) - 0.5f;
                NormalizeVector(Velocity.m_value);

                Timer.m_value = (std::rand() / (float)RAND_MAX) * 8;
            });

    }
}

void timer(int value)
{
    // Post re-paint request to activate display()
    glutPostRedisplay();

    // next timer call milliseconds later
    glutTimerFunc(15, timer, 0);
}
int main(int argc, char** argv)
{
    InitializeGame();
    glutInitWindowSize(s_Game.m_w, s_Game.m_h);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutCreateWindow("ECS");
    glutDisplayFunc([]
        {
            s_Game.m_ecsMgr->Run();
        });
    glutReshapeFunc([](int w, int h)
        {
            s_Game.m_w = w;
            s_Game.m_h = h;
            glViewport(0, 0, w, h);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(0, w, 0, h, -1, 1);
            glScalef(1, -1, 1);
            glTranslatef(0, -h, 0);
        });
    glutTimerFunc(0, timer, 0);
    glutMainLoop();


}