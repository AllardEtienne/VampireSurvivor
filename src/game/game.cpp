#include "game/game.h"
#include "common/common.h"

#include "ecs/core.hpp"
#include "ecs/internal/system_manager.hpp"

#include <iostream>
#include <algorithm> 

enum class directionMe
{
    Up,
    Down,
    Left,
    Right,
    Idle
};

struct Position : public common::VectorMe
{
};

struct Motion
{
    float vx = 0.f;
    float vy = 0.f;
    float speed = 5.f;
};

struct Hitbox
{
    float width, height;
};

struct RanderShape
{
    sf::RectangleShape shape;
    sf::Color color;
};

struct Direction
{
    directionMe dir;
};

struct follow
{
    ecs::Entity target;
};

class followSystem : public ecs::System
{
    public:
    void updateFollow()
    {
        for (ecs::Entity entity : entities())
        {
            auto& pos = ecs::get_component<Position>(entity);
            auto& mot = ecs::get_component<Motion>(entity);
            auto& followC = ecs::get_component<follow>(entity);

            auto& targetPos = ecs::get_component<Position>(followC.target);
            float dx = targetPos.x - pos.x;
            float dy = targetPos.y - pos.y;
            float length = std::sqrt(dx * dx + dy * dy);
            if (length > 0.f)
            {
                mot.vx = dx / length;
                mot.vy = dy / length;
            }
            else
            {
                mot.vx = 0.f;
                mot.vy = 0.f;
            }
        }
    }
};

class inputSystem : public ecs::System
{
public:
    void processInput(directionMe newDirection)
    {
        for (ecs::Entity entity : entities())
        {
            auto& direction = ecs::get_component<Direction>(entity);
            auto& motion = ecs::get_component<Motion>(entity);
            direction.dir = newDirection;

            if (direction.dir == directionMe::Down)
            {
                motion.vx = 0.f;
                motion.vy = 1.f;
            }
            else if (direction.dir == directionMe::Up)
            {
                motion.vx = 0.f;
                motion.vy = -1.f;
            }
            else if (direction.dir == directionMe::Right)
            {
                motion.vx = 1.f;
                motion.vy = 0.f;
            }
            else if (direction.dir == directionMe::Left)
            {
                motion.vx = -1.f;
                motion.vy = 0.f;
            }
            else if (direction.dir == directionMe::Idle)
            {
                motion.vx = 0.f;
                motion.vy = 0.f;
            }
        }
    }
};

class ColisionSystem : public ecs::System
{
public:
    bool CheckCollisions(ecs::Entity entityCheck)
    {
        auto& posCheck = ecs::get_component<Position>(entityCheck);
        auto& hbCheck = ecs::get_component<Hitbox>(entityCheck);

        for (ecs::Entity entity : entities())
        {
            if (entity == entityCheck)
                continue;

            auto& pos = ecs::get_component<Position>(entity);
            auto& hb = ecs::get_component<Hitbox>(entity);

            bool collision = pos.x < posCheck.x + hbCheck.width && pos.x + hb.width > posCheck.x &&
                             pos.y < posCheck.y + hbCheck.height && pos.y + hb.height > posCheck.y;

            if (collision)
            {
                return true;
            }
        }
        return false;
    }

    void CheckCillisionsRemove(ecs::Entity entityCheck)
    {
        auto& posCheck = ecs::get_component<Position>(entityCheck);
        auto& hbCheck = ecs::get_component<Hitbox>(entityCheck);

        for (ecs::Entity entity : entities())
        {
            if (entity == entityCheck)
                continue;

            auto& pos = ecs::get_component<Position>(entity);
            auto& hb = ecs::get_component<Hitbox>(entity);

            bool collision = pos.x < posCheck.x + hbCheck.width && pos.x + hb.width > posCheck.x &&
                             pos.y < posCheck.y + hbCheck.height && pos.y + hb.height > posCheck.y;

            if (collision)
            {
                ecs::destroy_entity(entity);
            }
        }
        
    }

    void CheckQuitWindow( sf::RenderWindow& window)
    {
        for (ecs::Entity entity : entities())
        {
            auto& pos = ecs::get_component<Position>(entity);
            auto& hb = ecs::get_component<Hitbox>(entity);
            if (pos.x < 0.f || pos.x + hb.width > static_cast<float>(game::ui::widthWindow) ||
                pos.y < 0.f || pos.y + hb.height > static_cast<float>(game::ui::heightWindow))
            {
                ecs::destroy_entity(entity);
            }
        }
    }


};

class MovementSystem : public ecs::System
{
public:
    void updatePositions()
    {
        for (ecs::Entity entity : entities())
        {
            auto& pos = ecs::get_component<Position>(entity);
            auto& mot = ecs::get_component<Motion>(entity);
            // Appliquer la translation en utilisant la direction normalisée multipliée par la vitesse.
            pos.x += mot.vx * mot.speed;
            pos.y += mot.vy * mot.speed;
        }
    }
};

class RenderSystem : public ecs::System
{
public:
    void renderEntities(sf::RenderWindow& window)
    {
        for (ecs::Entity entity : entities())
        {
            auto& renderShape = ecs::get_component<RanderShape>(entity);
            auto& position = ecs::get_component<Position>(entity);
            renderShape.shape.setPosition(position.x, position.y);
            renderShape.shape.setFillColor(renderShape.color);
            window.draw(renderShape.shape);
        }
    }
};




namespace game::ui
{

void spawnPoint(float& y, float& x)
{
    float spawn = common::random_float(0.f, 4.f);

    if (spawn < 1.f)
    {
        // Spawn en haut
        y = 0.f;
        x = common::random_float(0.f, std::max(0, widthWindow));
    }
    else if (spawn < 2.f)
    {
        // Spawn en bas
        y = static_cast<float>(heightWindow - 50.0f);
        x = common::random_float(0.f, std::max(0, widthWindow));
    }
    else if (spawn < 3.f)
    {
        // Spawn à gauche
        x = 0.f;
        y = common::random_float(0.f, std::max(0, heightWindow));
    }
    else
    {
        // Spawn à droite
        x = static_cast<float>(widthWindow - 50.0f);
        y = common::random_float(0.f, std::max(0, heightWindow));
    }
}

void projectileDestination(float& vx, float& vy, float xSpawn, float ySpawn)
{
    const float eps = 0.1f;
    const float projSize = 50.0f;

    float targetX = xSpawn;
    float targetY = ySpawn;

    if (xSpawn >= static_cast<float>(widthWindow) - projSize - eps) 
    {
        // Spawn droite -> viser gauche
        targetX = 0.f;
        targetY = common::random_float(0.f, static_cast<float>(heightWindow));
    }
    else if (xSpawn <= eps) 
    {
        // Spawn gauche -> viser droite
        targetX = static_cast<float>(widthWindow);
        targetY = common::random_float(0.f, static_cast<float>(heightWindow));
    }
    else if (ySpawn <= eps) 
    {
        // Spawn haut -> viser bas
        targetY = static_cast<float>(heightWindow);
        targetX = common::random_float(0.f, static_cast<float>(widthWindow));
    }
    else if (ySpawn >= static_cast<float>(heightWindow) - projSize - eps) 
    {
        // Spawn bas -> viser haut
        targetY = 0.f;
        targetX = common::random_float(0.f, static_cast<float>(widthWindow));
    }
    else
    {
        vx = 0.f;
        vy = 0.f;
        return;
    }

    float dx = targetX - xSpawn;
    float dy = targetY - ySpawn;
    float length = std::sqrt(dx * dx + dy * dy);

    if (length > 0.f)
    {
        vx = (dx / length);
        vy = (dy / length);
    }
    else
    {
        vx = 0.f;
        vy = 0.f;
    }
}

ecs::Entity spawnProjectile( sf::Color colors)
{
    float y, x, vx, vy;

        spawnPoint(y, x);
        projectileDestination(vx, vy, x, y);
        ecs::Entity proj = ecs::create_entity();

        sf::RectangleShape shape(sf::Vector2f(50.f, 50.f));
        shape.setFillColor(colors);
        shape.setPosition(x, y);

        ecs::add_components(proj, Position{x, y}, Hitbox{50.f, 50.f}, RanderShape{shape, colors}, Motion{vx, vy, 3.0f});
        return proj;
}

ecs::Entity spawnEnnemy(sf::Color colors, ecs::Entity target)
{
    float y, x;

        spawnPoint(y, x);
        ecs::Entity enemy = ecs::create_entity();

        sf::RectangleShape shape(sf::Vector2f(50.f, 50.f));
        shape.setFillColor(colors);
        shape.setPosition(x, y);

        ecs::add_components(enemy, Position{x, y},  Hitbox{50.f, 50.f}, RanderShape{shape, colors},  Motion{0.0f, 0.0f, 2.0f}, follow{target});
        return enemy;
}



void registerSystems()
{
    ecs::register_component<Position>();
    ecs::register_component<Motion>();
    ecs::register_component<Hitbox>();
    ecs::register_component<RanderShape>();
    ecs::register_component<Direction>();
    ecs::register_component<follow>();

    auto movement = std::make_shared<MovementSystem>();
    ecs::register_system<MovementSystem>(movement, ecs::create_signature<Position, Motion>());

    auto colision = std::make_shared<ColisionSystem>();
    ecs::register_system<ColisionSystem>(colision, ecs::create_signature<Position, Hitbox>());

    auto render = std::make_shared<RenderSystem>();
    ecs::register_system<RenderSystem>(render, ecs::create_signature<Position, RanderShape>());

    auto input = std::make_shared<inputSystem>();
    ecs::register_system<inputSystem>(input, ecs::create_signature<Direction, Motion>());

    auto followS = std::make_shared<followSystem>();
    ecs::register_system<followSystem>(followS, ecs::create_signature<Position, Motion, follow>());
};

/////////////
//RUN GAME//
///////////

std::expected<int, std::string> runGame()
{
    registerSystems();

    directionMe lastDirection = directionMe::Idle;
    directionMe currentDirection = directionMe::Idle;
    directionMe lookingDirection = directionMe::Down;
    int frameCounter = 0;
    
    sf::RectangleShape playerShape;
    playerShape.setFillColor(sf::Color::White);
    playerShape.setSize(sf::Vector2f(50.f, 50.f));
    playerShape.setPosition(widthWindow / 2.f, heightWindow / 2.f);


    ecs::Entity player = ecs::create_entity();
    ecs::add_components(player, Position{widthWindow / 2.f, heightWindow / 2.f}, Motion{0.f, 0.f}, Hitbox{50.f, 50.f},
                        RanderShape{playerShape, sf::Color::White}, Direction{directionMe::Idle});

    
    // liste des projectiles
    std::vector<ecs::Entity> projectiles;
    for (int i = 0; i < game::ui::numberOfProjectile; ++i)
    projectiles.push_back(spawnProjectile(sf::Color::Red));

    std::vector<ecs::Entity> ennemies;
    for (int i = 0; i < game::ui::numberOfEnnemies; ++i)
    ennemies.push_back(spawnEnnemy(sf::Color::Blue , player));


    // TEXTURE PLAYER //
    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("assets/player.png"))
    {
        return std::unexpected("Erreur lors du chargement de la texture");
    }

    playerTexture.setSmooth(true);
    sf::Sprite playerSprite;
    playerSprite.setTexture(playerTexture);
    playerSprite.setPosition(widthWindow / 2.f, heightWindow / 2.f);
    // FIN TEXTURE PLAYER //




    // Création d'une fenêtre
    sf::RenderWindow window(sf::VideoMode(widthWindow, heightWindow), "VampireSurvivor",
                            sf::Style::Default); // Pour un style personnalisé

    window.setFramerateLimit(30);

    // Boucle principale avec gestion frame par frame
    while (window.isOpen())
    {
        // Gestion des évènements
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Demande de fermeture (croix de la fenêtre, ALT+F4)
            if (event.type == sf::Event::Closed)
            {
                window.close();
                return std::unexpected("fermeture de la fenetre");
            }

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                {
                    window.close();
                    return std::unexpected("fermeture de la fenetre avec echap");
                }
                else if (event.key.code == sf::Keyboard::D or event.key.code == sf::Keyboard::Right)
                {
                    currentDirection = directionMe::Right;
                    lookingDirection = directionMe::Right;
                    ecs::get_system<inputSystem>()->processInput(directionMe::Right);

                }
                else if (event.key.code == sf::Keyboard::Q or event.key.code == sf::Keyboard::Left)
                {
                    currentDirection = directionMe::Left;
                    lookingDirection = directionMe::Left;
                    ecs::get_system<inputSystem>()->processInput(directionMe::Left);

                }
                else if (event.key.code == sf::Keyboard::Z or event.key.code == sf::Keyboard::Up)
                {
                    currentDirection = directionMe::Up;
                    lookingDirection = directionMe::Up;
                    ecs::get_system<inputSystem>()->processInput(directionMe::Up);

                }
                else if (event.key.code == sf::Keyboard::S or event.key.code == sf::Keyboard::Down)
                {
                    currentDirection = directionMe::Down;
                    lookingDirection = directionMe::Down;
                    ecs::get_system<inputSystem>()->processInput(directionMe::Down);

                }
                else
                {
                    currentDirection = directionMe::Idle;
                    ecs::get_system<inputSystem>()->processInput(directionMe::Idle);

                }
            }
        }


        ecs::get_system<followSystem>()->updateFollow();

        ecs::get_system<MovementSystem>()->updatePositions();

        ecs::get_system<ColisionSystem>()->CheckQuitWindow(window);

        auto cleanup_list = [&](std::vector<ecs::Entity> &list) {
            list.erase(std::remove_if(list.begin(), list.end(), [](const ecs::Entity &e) {
                return !ecs::has_component<Position>(e);
            }), list.end());
        };

        cleanup_list(projectiles);
        cleanup_list(ennemies);

        ecs::get_system<ColisionSystem>()->CheckCillisionsRemove(player);

        cleanup_list(projectiles);
        cleanup_list(ennemies);

        ecs::get_system<inputSystem>()->processInput(directionMe::Idle);


        // respawn projectiles
        if (projectiles.size() < game::ui::numberOfProjectile)
        {
            projectiles.push_back(spawnProjectile(sf::Color::Red));
        }
        if (ennemies.size() < game::ui::numberOfEnnemies)
        {
            ennemies.push_back(spawnEnnemy(sf::Color::Blue, player));
        }


        // Effacement de l'ancienne frame (framebuffer)
        window.setVerticalSyncEnabled(true);

        window.clear(sf::Color::Green);

        // Dessiner les éléments du menu
        if (currentDirection == lastDirection)
        {
            frameCounter++;
        }
        else
        {
            frameCounter = 0;
        }

        if (currentDirection == directionMe::Right)
        {
            playerSprite.setTextureRect(sf::IntRect((frameCounter / 10) % 4 * 32, 32 * 9, 32, 32));
        }
        else if (currentDirection == directionMe::Left)
        {
            playerSprite.setTextureRect(sf::IntRect((frameCounter / 10) % 4 * 32, 32 * 7, 32, 32));
        }
        else if (currentDirection == directionMe::Up)
        {
            playerSprite.setTextureRect(sf::IntRect((frameCounter / 10) % 4 * 32, 32 * 11, 32, 32));
        }
        else if (currentDirection == directionMe::Down)
        {
            playerSprite.setTextureRect(sf::IntRect((frameCounter / 10) % 4 * 32, 32 * 5, 32, 32));
        }
        else if (currentDirection == directionMe::Idle)
        {
            if (lookingDirection == directionMe::Right)
            {
                playerSprite.setTextureRect(sf::IntRect((frameCounter / 10) % 4 * 32, 32 * 1, 32, 32));
            }
            else if (lookingDirection == directionMe::Left)
            {
                playerSprite.setTextureRect(sf::IntRect((frameCounter / 10) % 4 * 32, 32 * 2, 32, 32));
            }
            else if (lookingDirection == directionMe::Up)
            {
                playerSprite.setTextureRect(sf::IntRect((frameCounter / 10) % 4 * 32, 32 * 3, 32, 32));
            }
            else if (lookingDirection == directionMe::Down)
            {
                playerSprite.setTextureRect(sf::IntRect((frameCounter / 10) % 4 * 32, 32 * 0, 32, 32));
            }
        }
        else

            playerSprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
        playerSprite.setScale(2.f, 2.f);

        window.draw(playerSprite);

        ecs::get_system<RenderSystem>()->renderEntities(window);


        // Affiche la nouvelle frame à l'écran
        window.display();
        lastDirection = currentDirection;
    };
    return 0;
};
}; // namespace game::ui
