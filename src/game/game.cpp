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
int widthWindow = 800, heightWindow = 600;

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



 std::expected<int, std::string> runGame()
{
     direction lastDirection = direction::Idle;
     direction currentDirection = direction::Idle;
     direction lookingDirection = direction::Down;
     int frameCounter = 0;
     

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


            // D'autres évènements peuvent être gérés ici
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

        // Affiche la nouvelle frame à l'écran
        window.display();
        lastDirection = currentDirection;
        
    }
    return 0;
}
} 
