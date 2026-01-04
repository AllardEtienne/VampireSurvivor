#include "game/game.h"
#include "common/common.h"

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

void spawnEnemy(float& y, float& x)
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
    else if (ySpawn >=0 and ySpawn <= heightWindow)
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
    else if (xSpawn >=0 and xSpawn <= widthWindow)
    {
        xDestination = common::random_float(0, widthWindow);
    }
}


void shootProjectile(std::vector<common::Ennemy>& projectiles, int count = 1, sf::Color colors = sf::Color::White)
{
    float y;
    float x;
    float yDestination = heightWindow / 2.f;
    float xDestination = widthWindow / 2.f;
    const float radius = 25.f;
    const float spacing = 10.f;                     // espacement horizontal entre projectiles

    // Prendre un peu d'avance pour éviter reallocation fréquente si on tire beaucoup
    if (projectiles.capacity() < projectiles.size() + count)
        projectiles.reserve(projectiles.size() + count);
    
    for (int i = 0; i < count; ++i)
    {
        spawnEnemy(y, x);
        projectileDestination(yDestination, xDestination, y, x);
        sf::CircleShape proj(radius);
        proj.setFillColor(colors);
        proj.setPosition(x, y);
        common::Ennemy p(y, x, yDestination, xDestination, proj);
        projectiles.emplace_back(std::move(p));
    }
}

void moveTo(std::vector<common::Ennemy>& projectiles, float speed)
{
    for (auto& projectile : projectiles)
    {
        projectile.moveTowardsDestination(speed);
    }
}





 std::expected<int, std::string> runGame()
{
     float yDestination;
     float xDestination;
     spawnEnemy(yDestination, xDestination);


     direction lastDirection = direction::Idle;
     direction currentDirection = direction::Idle;
     direction lookingDirection = direction::Down;
     int frameCounter = 0;
     
     std::vector<common::Ennemy> projectiles;
     
     shootProjectile(projectiles, 10, sf::Color::Red);

     std::vector<common::Ennemy> ennemies;
     shootProjectile(ennemies, 5, sf::Color::Blue);

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
        moveTo(projectiles, 1.f);

        for (auto it = projectiles.begin(); it != projectiles.end();)
        {
            if (it->destination_reached())
            {
                it = projectiles.erase(it); // erase retourne l’itérateur suivant
                //shootProjectile(projectiles, 1, sf::Color::Red);
            }
            else if (it->getPosition().x < playerSprite.getPosition().x + 25 and
                     it->getPosition().x > playerSprite.getPosition().x - 25 and
                     it->getPosition().y < playerSprite.getPosition().y + 25 and
                     it->getPosition().y > playerSprite.getPosition().y - 25)
                {
                    it = projectiles.erase(it); // erase retourne l’itérateur suivant
                    //shootProjectile(projectiles, 1, sf::Color::Red);
                }
            else
            {
                ++it;
            }
        }

        for (auto it = ennemies.begin(); it != ennemies.end();)
        {
            if (it->destination_reached())
            {
                it = ennemies.erase(it); // erase retourne l’itérateur suivant
                //shootProjectile(ennemies, 1, sf::Color::Blue);
            }
            else if (it->getPosition().x < playerSprite.getPosition().x + 25 and
                     it->getPosition().x > playerSprite.getPosition().x - 25 and
                     it->getPosition().y < playerSprite.getPosition().y + 25 and
                     it->getPosition().y > playerSprite.getPosition().y - 25)
                {
                    it = ennemies.erase(it); // erase retourne l’itérateur suivant
                    //shootProjectile(ennemies, 1, sf::Color::Blue);
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


        for (const auto& projectile : projectiles)
        {
            projectile.drawEnnemy(window);
        }
        for (const auto& ennemy : ennemies)
        {
            ennemy.drawEnnemy(window);
        }


        // Affiche la nouvelle frame à l'écran
        window.display();
        lastDirection = currentDirection;
        
    }
    return 0;
}
} 
