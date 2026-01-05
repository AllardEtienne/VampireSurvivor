#include "game/game.h"
#include "common/common.h"

#include "ecs/core.hpp"
#include "ecs/internal/system_manager.hpp"


struct Position : public common::VectorMe
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

struct RanderShape
{
    sf::RectangleShape shape;
    sf::Color color;
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
    void updatePositions()
    {
        for (ecs::Entity entity : entities())
        {
            auto& pos = ecs::get_component<Position>(entity);
            auto& mot = ecs::get_component<Motion>(entity);
            pos.x += mot.vx;
            pos.y += mot.vy;
        }
    }

    void moveDestination(float yDestination, float xDestination)
    {
        for (ecs::Entity entity : entities())
        {
            auto& pos = ecs::get_component<Position>(entity);
            auto& mot = ecs::get_component<Motion>(entity);

            float dx = xDestination - pos.x;
            float dy = yDestination - pos.y;
            float dist2 = dx * dx + dy * dy;

            const float eps = 1e-4f;
            if (dist2 <= eps * eps)
            {
                // Proche de la destination : stopper le mouvement
                mot.vx = 0.f;
                mot.vy = 0.f;
                continue;
            }

            float invDist = 1.0f / std::sqrt(dist2);
            // Vitesse normalisée vers la destination (direction unitaire).
            mot.vx = dx * invDist;
            mot.vy = dy * invDist;
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



enum class direction
{
    Up,
    Down,
    Left,
    Right,
    Idle
};


namespace game::ui
{
int widthWindow = 1000, heightWindow = 800;

void moveUp(sf::Sprite& sprite)
{
    sprite.move(0.f, -5.f);
}
void moveDown(sf::Sprite& sprite)
{
    sprite.move(0.f, 5.f);
}
void moveLeft(sf::Sprite& sprite)
{
    sprite.move(-5.f, 0.f);
}
void moveRight(sf::Sprite& sprite)
{
    sprite.move(5.f, 0.f);
}

void spawnPoint(float& y, float& x)
{
    float spawn = common::random_float(0.f, 4.f);

    if (spawn < 1.f)
    {
        // Haut
        y = 0;
        x = common::random_float(0, widthWindow);
    }
    else if (spawn < 2.f)
    {
        // Bas
        y = heightWindow;
        x = common::random_float(0, widthWindow);
    }
    else if (spawn < 3.f)
    {
        // Gauche
        x = 0;
        y = common::random_float(0, heightWindow);
    }
    else
    {
        // Droite
        x = widthWindow;
        y = common::random_float(0, heightWindow);
    }
}

void projectileDestination(float& yDestination, float& xDestination, float ySpawn, float xSpawn)
{
    if (ySpawn == 0.f)
    {
        // Haut
        yDestination = heightWindow;
    }
    else if (ySpawn == heightWindow)
    {
        // Bas
        yDestination = 0.f;
    }
    else if (ySpawn >= 0 and ySpawn <= heightWindow)
    {
        yDestination = common::random_float(0, heightWindow);
    }

    if (xSpawn == 0.f)
    {
        // Gauche
        xDestination = widthWindow;
    }
    else if (xSpawn == widthWindow)
    {
        // Droite
        xDestination = 0.f;
    }
    else if (xSpawn >= 0 and xSpawn <= widthWindow)
    {
        xDestination = common::random_float(0, widthWindow);
    }
}

void spawnEnnemy(std::vector<common::Ennemy>& projectiles, int count = 1, sf::Color colors = sf::Color::White)
{
    float y;
    float x;
    float yDestination = heightWindow / 2.f;
    float xDestination = widthWindow / 2.f;
    const float radius = 25.f;
    const float spacing = 10.f; // espacement horizontal entre projectiles

    // Prendre un peu d'avance pour éviter reallocation fréquente si on tire beaucoup
    if (projectiles.capacity() < projectiles.size() + count)
        projectiles.reserve(projectiles.size() + count);

    for (int i = 0; i < count; ++i)
    {
        spawnPoint(y, x);
        projectileDestination(yDestination, xDestination, y, x);
        sf::CircleShape proj(radius);
        proj.setFillColor(colors);
        proj.setPosition(x, y);
        common::Ennemy p(y, x, yDestination, xDestination, proj);
        projectiles.emplace_back(std::move(p));
    }
}

void spawnEnnemyBis(std::vector<ecs::Entity>& projectiles, int count = 1, sf::Color colors = sf::Color::White)
{
    float y;
    float x;


    for (int i = 0; i < count; ++i)
    {
        spawnPoint(y, x);
        ecs::Entity proj = ecs::create_entity();
        sf::RectangleShape shape(sf::Vector2f(50.f, 50.f));
        shape.setFillColor(colors);
        shape.setPosition(y, x);
        sf::RectangleShape box(sf::Vector2f(50.f, 50.f));
        box.setPosition(y, x);
        box.setFillColor(sf::Color::Transparent);
        ecs::add_components(proj, Position{y, x}, Hitbox{box}, RanderShape{shape});
        projectiles.emplace_back(proj);
    }
}

void moveTo(std::vector<common::Ennemy>& projectiles, float speed)
{
    for (auto& projectile : projectiles)
    {
        projectile.moveTowardsDestination(speed);
    }
}

void registerSystems()
{
    ecs::register_component<Position>();
    ecs::register_component<Motion>();
    ecs::register_component<Hitbox>();
    ecs::register_component<RanderShape>();

    auto movement = std::make_shared<MovementSystem>();
    ecs::register_system<MovementSystem>(movement, ecs::create_signature<Position, Motion>());

    auto colision = std::make_shared<ColisionSystem>();
    ecs::register_system<ColisionSystem>(colision, ecs::create_signature<Position, Hitbox>());

    auto render = std::make_shared<RenderSystem>();
    ecs::register_system<RenderSystem>(render, ecs::create_signature<Position, RanderShape>());


};

std::expected<int, std::string> runGame()
{
    registerSystems();

    float yDestination;
    float xDestination;
    spawnPoint(yDestination, xDestination);

    direction lastDirection = direction::Idle;
    direction currentDirection = direction::Idle;
    direction lookingDirection = direction::Down;
    int frameCounter = 0;

    std::vector<ecs::Entity> projectiles;

    spawnEnnemyBis(projectiles, 10, sf::Color::Red);

    std::vector<common::Ennemy> ennemies;
    spawnEnnemy(ennemies, 5, sf::Color::Blue);

    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("assets/player.png"))
    {
        return std::unexpected("Erreur lors du chargement de la texture");
    }

    playerTexture.setSmooth(true);
    sf::Sprite playerSprite;
    playerSprite.setTexture(playerTexture);
    playerSprite.setPosition(widthWindow / 2.f, heightWindow / 2.f);

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
                    currentDirection = direction::Right;
                    lookingDirection = direction::Right;
                    moveRight(playerSprite);
                }
                else if (event.key.code == sf::Keyboard::Q or event.key.code == sf::Keyboard::Left)
                {
                    currentDirection = direction::Left;
                    lookingDirection = direction::Left;
                    moveLeft(playerSprite);
                }
                else if (event.key.code == sf::Keyboard::Z or event.key.code == sf::Keyboard::Up)
                {
                    currentDirection = direction::Up;
                    lookingDirection = direction::Up;
                    moveUp(playerSprite);
                }
                else if (event.key.code == sf::Keyboard::S or event.key.code == sf::Keyboard::Down)
                {
                    currentDirection = direction::Down;
                    lookingDirection = direction::Down;
                    moveDown(playerSprite);
                }
                else
                {
                    currentDirection = direction::Idle;
                }
            }
        }
        for (auto& ennemy : ennemies)
        {
            ennemy.changeDestination(playerSprite.getPosition().y, playerSprite.getPosition().x);
        }
        moveTo(ennemies, 2.f);

        for (auto& projectile : projectiles)
        {
            ;
        }

        // moveTo(projectiles, 1.f);

        // for (auto it = projectiles.begin(); it != projectiles.end();)
        //{
        //     if (it->destination_reached())
        //     {
        //         it = projectiles.erase(it); // erase retourne l’itérateur suivant
        //         //spawnEnnemy(projectiles, 1, sf::Color::Red);
        //     }
        //     else if (it->getPosition().x < playerSprite.getPosition().x + 25 and
        //              it->getPosition().x > playerSprite.getPosition().x - 25 and
        //              it->getPosition().y < playerSprite.getPosition().y + 25 and
        //              it->getPosition().y > playerSprite.getPosition().y - 25)
        //         {
        //             it = projectiles.erase(it); // erase retourne l’itérateur suivant
        //             //spawnEnnemy(projectiles, 1, sf::Color::Red);
        //         }
        //     else
        //     {
        //         ++it;
        //     }
        // }

        for (auto it = ennemies.begin(); it != ennemies.end();)
        {
            if (it->destination_reached())
            {
                it = ennemies.erase(it); // erase retourne l’itérateur suivant
                // spawnEnnemy(ennemies, 1, sf::Color::Blue);
            }
            else if (it->getPosition().x < playerSprite.getPosition().x + 25 and
                     it->getPosition().x > playerSprite.getPosition().x - 25 and
                     it->getPosition().y < playerSprite.getPosition().y + 25 and
                     it->getPosition().y > playerSprite.getPosition().y - 25)
            {
                it = ennemies.erase(it); // erase retourne l’itérateur suivant
                // spawnEnnemy(ennemies, 1, sf::Color::Blue);
            }
            else
            {
                ++it;
            }
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

        if (currentDirection == direction::Right)
        {
            playerSprite.setTextureRect(sf::IntRect((frameCounter / 10) % 4 * 32, 32 * 9, 32, 32));
        }
        else if (currentDirection == direction::Left)
        {
            playerSprite.setTextureRect(sf::IntRect((frameCounter / 10) % 4 * 32, 32 * 7, 32, 32));
        }
        else if (currentDirection == direction::Up)
        {
            playerSprite.setTextureRect(sf::IntRect((frameCounter / 10) % 4 * 32, 32 * 11, 32, 32));
        }
        else if (currentDirection == direction::Down)
        {
            playerSprite.setTextureRect(sf::IntRect((frameCounter / 10) % 4 * 32, 32 * 5, 32, 32));
        }
        else if (currentDirection == direction::Idle)
        {
            if (lookingDirection == direction::Right)
            {
                playerSprite.setTextureRect(sf::IntRect((frameCounter / 10) % 4 * 32, 32 * 1, 32, 32));
            }
            else if (lookingDirection == direction::Left)
            {
                playerSprite.setTextureRect(sf::IntRect((frameCounter / 10) % 4 * 32, 32 * 2, 32, 32));
            }
            else if (lookingDirection == direction::Up)
            {
                playerSprite.setTextureRect(sf::IntRect((frameCounter / 10) % 4 * 32, 32 * 3, 32, 32));
            }
            else if (lookingDirection == direction::Down)
            {
                playerSprite.setTextureRect(sf::IntRect((frameCounter / 10) % 4 * 32, 32 * 0, 32, 32));
            }
        }
        else

            playerSprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
        playerSprite.setScale(2.f, 2.f);

        window.draw(playerSprite);

        ecs::get_system<RenderSystem>()->renderEntities(window);

        for (const auto& ennemy : ennemies)
        {
            ennemy.drawEnnemy(window);
        }

        // Affiche la nouvelle frame à l'écran
        window.display();
        lastDirection = currentDirection;
    };
    return 0;
};
}; // namespace game::ui
