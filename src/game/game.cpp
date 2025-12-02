#include "game/game.h"
#include "common/common.h"


namespace game::ui
{
int widthWindow = 800, heightWindow = 600;

void moveUp(sf::Sprite& sprite)
{
    sprite.move(0.f, -5.f);
    sprite.setTextureRect(sf::IntRect(0, 352, 32, 32));
}
void moveDown(sf::Sprite& sprite)
{
    sprite.move(0.f, 5.f);
    sprite.setTextureRect(sf::IntRect(0, 160, 32, 32));
}
void moveLeft(sf::Sprite& sprite)
{
    sprite.move(-5.f, 0.f);
    sprite.setTextureRect(sf::IntRect(0, 224, 32, 32));
}
void moveRight(sf::Sprite& sprite)
{
    sprite.move(5.f, 0.f);
    sprite.setTextureRect(sf::IntRect(0, 288, 32, 32));
}
void idle(sf::Sprite& sprite)
{
    sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
}



 std::expected<int, std::string> runGame()
{
    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("assets/player.png"))
    {
        return std::unexpected("Erreur lors du chargement de la texture");
    }

    playerTexture.setSmooth(true);
    sf::Sprite playerSprite;
    playerSprite.setTexture(playerTexture);
    playerSprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
    playerSprite.setScale(2.f, 2.f);
    playerSprite.setPosition(widthWindow / 2.f, heightWindow / 2.f);
    



    // Création d'une fenêtre "render"
    sf::RenderWindow window(sf::VideoMode(widthWindow, heightWindow), "VampireSurvivor",
                            sf::Style::Default); // Pour un style personnalisé
    
    window.setFramerateLimit(60);


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
                    moveRight(playerSprite);
                }
                else if (event.key.code == sf::Keyboard::Q or event.key.code == sf::Keyboard::Left)
                {
                    moveLeft(playerSprite);
                }
                else if (event.key.code == sf::Keyboard::Z or event.key.code == sf::Keyboard::Up)
                {
                    moveUp(playerSprite);
                }
                else if (event.key.code == sf::Keyboard::S or event.key.code == sf::Keyboard::Down)
                {
                    moveDown(playerSprite);
                }
                else
                {
                    idle(playerSprite);
                }
            }

            // D'autres évènements peuvent être gérés ici
        }

        // Effacement de l'ancienne frame (framebuffer)
        window.setVerticalSyncEnabled(true);

        window.clear(sf::Color::Green);

        // Dessiner les éléments du menu
        window.draw(playerSprite);

        // Affiche la nouvelle frame à l'écran
        window.display();
    }
    return 0;
}
} 
