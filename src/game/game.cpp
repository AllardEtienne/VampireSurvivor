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

struct Follow
{
    ecs::Entity target;
};

struct Sprite
{
    sf::Sprite sprite;
    int frameCounter = 0;
};

struct Direction
{
    directionMe currentDirection;
    directionMe lastDirection;
    directionMe movementDirection;
};

struct Enemy
{
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
            auto& followC = ecs::get_component<Follow>(entity);

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
            direction.movementDirection = newDirection;

            if (direction.movementDirection == directionMe::Down)
            {
                motion.vx = 0.f;
                motion.vy = 1.f;
                direction.currentDirection = directionMe::Down;
            }
            else if (direction.movementDirection == directionMe::Up)
            {
                motion.vx = 0.f;
                motion.vy = -1.f;
                direction.currentDirection = directionMe::Up;
            }
            else if (direction.movementDirection == directionMe::Right)
            {
                motion.vx = 1.f;
                motion.vy = 0.f;
                direction.currentDirection = directionMe::Right;
            }
            else if (direction.movementDirection == directionMe::Left)
            {
                motion.vx = -1.f;
                motion.vy = 0.f;
                direction.currentDirection = directionMe::Left;
            }
            else if (direction.movementDirection == directionMe::Idle)
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
class RenderSystemEnemy : public ecs::System
{
    public:
    void renderEntities(sf::RenderWindow& window)
    {
        for (ecs::Entity entity : entities())
        {
            auto& position = ecs::get_component<Position>(entity);
            auto& sprite = ecs::get_component<Sprite>(entity);
            int& frameCounter = sprite.frameCounter;

            frameCounter++;
            sprite.sprite.setTextureRect(sf::IntRect((frameCounter / 10) % 4 * 32, 32 * 0, 32, 32));
            

            sprite.sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
            sprite.sprite.setScale(2.f, 2.f);
            sprite.sprite.setPosition(position.x, position.y);

            window.draw(sprite.sprite);
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
            auto& position = ecs::get_component<Position>(entity);
            auto& sprite = ecs::get_component<Sprite>(entity);
            auto& direction = ecs::get_component<Direction>(entity);
            directionMe currentDirection = direction.currentDirection;
            directionMe lastDirection = direction.lastDirection;
            int& frameCounter = sprite.frameCounter;


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
                sprite.sprite.setTextureRect(sf::IntRect((frameCounter / 10) % 4 * 32, 32 * 4, 32, 32));
            }
            else if (currentDirection == directionMe::Left)
            {
                sprite.sprite.setTextureRect(sf::IntRect((frameCounter / 10) % 4 * 32, 32 * 3, 32, 32));
            }
            else if (currentDirection == directionMe::Up)
            {
                sprite.sprite.setTextureRect(sf::IntRect((frameCounter / 10) % 4 * 32, 32 * 2, 32, 32));
            }
            else if (currentDirection == directionMe::Down)
            {
                sprite.sprite.setTextureRect(sf::IntRect((frameCounter / 10) % 4 * 32, 32 * 1, 32, 32));
            }
            else if (currentDirection == directionMe::Idle)
            {
                sprite.sprite.setTextureRect(sf::IntRect((frameCounter / 10) % 4 * 32, 32 * 0, 32, 32));
            }
            else

            sprite.sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
            sprite.sprite.setScale(2.f, 2.f);
            sprite.sprite.setPosition(position.x, position.y);

            window.draw(sprite.sprite);
            direction.lastDirection = direction.currentDirection;

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

ecs::Entity spawnProjectile(sf::Color colors)
{
    static sf::Texture projTexture;

    static bool loaded = false;
    if (!loaded)
    {
        if (!projTexture.loadFromFile("assets/projSprite.png"))
        {
            std::cerr << "Erreur chargement proj" << std::endl;
        }
        projTexture.setSmooth(true);
        loaded = true;
    }

    float y, x, vx, vy;
    spawnPoint(y, x);
    projectileDestination(vx, vy, x, y);

    ecs::Entity proj = ecs::create_entity();

    sf::Sprite projSprite;
    projSprite.setTexture(projTexture);

    ecs::add_components(proj, Position{x, y}, Hitbox{50.f, 50.f}, Sprite{projSprite}, Motion{vx, vy, 3.0f}, Enemy{});

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

        static sf::Texture enemyTexture;

        if (!enemyTexture.loadFromFile("assets/enemySprite.png"))
        {

        }

        enemyTexture.setSmooth(true);
        sf::Sprite enemySprite;
        enemySprite.setTexture(enemyTexture);
        enemySprite.setPosition(widthWindow / 2.f, heightWindow / 2.f);

        ecs::add_components(enemy, Position{x, y}, Hitbox{50.f, 50.f}, Sprite{enemySprite}, Motion{0.0f, 0.0f, 2.0f},
                            Follow{target}, Enemy{});
        return enemy;
}



void registerSystems()
{
    ecs::register_component<Position>();
    ecs::register_component<Motion>();
    ecs::register_component<Hitbox>();
    ecs::register_component<Direction>();
    ecs::register_component<Follow>();
    ecs::register_component<Sprite>();
    ecs::register_component<Enemy>();

    auto movement = std::make_shared<MovementSystem>();
    ecs::register_system<MovementSystem>(movement, ecs::create_signature<Position, Motion>());

    auto colision = std::make_shared<ColisionSystem>();
    ecs::register_system<ColisionSystem>(colision, ecs::create_signature<Position, Hitbox>());

    auto render = std::make_shared<RenderSystem>();
    ecs::register_system<RenderSystem>(render, ecs::create_signature<Position, Sprite, Direction>());

    auto renderNoDir = std::make_shared<RenderSystemEnemy>();
    ecs::register_system<RenderSystemEnemy>(renderNoDir, ecs::create_signature<Position, Sprite, Enemy>());

    auto input = std::make_shared<inputSystem>();
    ecs::register_system<inputSystem>(input, ecs::create_signature<Direction, Motion>());

    auto followS = std::make_shared<followSystem>();
    ecs::register_system<followSystem>(followS, ecs::create_signature<Position, Motion, Follow>());
};

/////////////
//RUN GAME//
///////////

std::expected<int, std::string> runGame()
{
    registerSystems();
    
    sf::RectangleShape playerShape;
    playerShape.setFillColor(sf::Color::White);
    playerShape.setSize(sf::Vector2f(50.f, 50.f));
    playerShape.setPosition(widthWindow / 2.f, heightWindow / 2.f);

    // TEXTURE PLAYER //
    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("assets/lolaSprite.png"))
    {
        return std::unexpected("Erreur lors du chargement de la texture");
    }

    playerTexture.setSmooth(true);
    sf::Sprite playerSprite;
    playerSprite.setTexture(playerTexture);
    playerSprite.setPosition(widthWindow / 2.f, heightWindow / 2.f);
    // FIN TEXTURE PLAYER //

    ecs::Entity player = ecs::create_entity();
    ecs::add_components(player, Position{widthWindow / 2.f, heightWindow / 2.f}, Motion{0.f, 0.f}, Hitbox{50.f, 50.f},
                        Sprite{playerSprite}, Direction{directionMe::Idle});

    
    // liste des projectiles
    std::vector<ecs::Entity> projectiles;
    for (int i = 0; i < game::ui::numberOfProjectile; ++i)
        projectiles.push_back(spawnProjectile(sf::Color::Red));

    std::vector<ecs::Entity> ennemies;
    for (int i = 0; i < game::ui::numberOfEnnemies; ++i)
        ennemies.push_back(spawnEnnemy(sf::Color::Blue , player));

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
                // On peut encore traiter des actions ponctuelles ici si besoin,
                // mais le mouvement sera géré chaque frame par l'état réel du clavier.
            }
        }

        // --- Gestion du mouvement par lecture en temps réel du clavier ---
        directionMe currentInput = directionMe::Idle;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            currentInput = directionMe::Right;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            currentInput = directionMe::Left;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            currentInput = directionMe::Up;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            currentInput = directionMe::Down;
        }
        else
        {
            currentInput = directionMe::Idle;
        }

        // Appel chaque frame pour garantir l'arrêt quand on relâche la touche
        ecs::get_system<inputSystem>()->processInput(currentInput);
        // --- fin gestion input temps réel ---

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

        ecs::get_system<RenderSystemEnemy>()->renderEntities(window);
        ecs::get_system<RenderSystem>()->renderEntities(window);


        // Affiche la nouvelle frame à l'écran
        window.display();
        
    };
    return 0;
};
}; // namespace game::ui
