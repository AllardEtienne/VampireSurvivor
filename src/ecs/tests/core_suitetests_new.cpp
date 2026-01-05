#include <gtest/gtest.h>
#include <SFML/Graphics.hpp>

#include "ecs/core.hpp"
#include "ecs/internal/system_manager.hpp"

namespace
{
struct Vector
{
    float x{}, y{};
};
;

struct Position : public Vector
{
    
};

struct Motion
{
    float vx, vy;
};

struct Hitbox
{
    sf::RectangleShape box;
};



class ColisionSystem : public ecs::System
{
public:
    bool CheckCollisions(ecs::Entity entityCheck)
    {
        for (ecs::Entity entity : entities())
        {
            if (entity == entityCheck)
                continue;
            auto& hitbox = ecs::get_component<Hitbox>(entity);
            auto& hitboxCheck = ecs::get_component<Hitbox>(entityCheck);
            if (hitbox.box.getGlobalBounds().intersects(hitboxCheck.box.getGlobalBounds()))
            {
                return true;
            }
        }
        return false;
    }
};

class MovementSystem : public ecs::System
{
public:
    void updatePositions(float dt)
    {
        for (ecs::Entity entity : entities())
        {
            auto& pos = ecs::get_component<Position>(entity);
            auto& mot = ecs::get_component<Motion>(entity);
            pos.x += mot.vx * dt;
            pos.y += mot.vy * dt;
        }
    }
};


} // namespace

TEST(CoreEcsTestSuite, MoveSystemUpdatePosition)
{
    ecs::register_component<Position>();
    ecs::register_component<Motion>();

    auto movement = std::make_shared<MovementSystem>();

    ecs::register_system<MovementSystem>(movement, ecs::create_signature<Position, Motion>());

    const ecs::Entity e = ecs::create_entity();
    ecs::add_components(e, Position{0.f, 0.f}, Motion{1.f, 1.f});
    movement->updatePositions(0.7f);
    const auto& pos = ecs::get_component<Position>(e);

    EXPECT_EQ(pos.x, 0.7f);
    EXPECT_EQ(pos.y, 0.7f);

    ecs::destroy_entity(e);
}

TEST(CoreEcsTestSuite, CollisionSystemDetectsCollision)
{
    ecs::register_component<Position>();
    ecs::register_component<Hitbox>();

    auto colision = std::make_shared<ColisionSystem>();

    ecs::register_system<ColisionSystem>(colision, ecs::create_signature<Position, Hitbox>());

    const ecs::Entity e1 = ecs::create_entity();
    sf::RectangleShape box1(sf::Vector2f(50.f, 50.f));
    box1.setPosition(100.f, 100.f);
    ecs::add_components(e1, Position{100.f, 100.f}, Hitbox{box1});

    const ecs::Entity e2 = ecs::create_entity();
    sf::RectangleShape box2(sf::Vector2f(50.f, 50.f));
    box2.setPosition(120.f, 120.f); // Overlaps with box1
    ecs::add_components(e2, Position{120.f, 120.f}, Hitbox{box2});

    bool collisionDetected = colision->CheckCollisions(e2);
    EXPECT_TRUE(collisionDetected);

    ecs::destroy_entity(e1);
    ecs::destroy_entity(e2);
}