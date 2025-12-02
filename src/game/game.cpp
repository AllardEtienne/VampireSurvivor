#include "game/game.h"
#include "common/common.h"
#include <SFML/Graphics.hpp>

namespace game::ui
{
int widthWindow = 800, heightWindow = 600;

 std::expected<int, std::string> runGame()
{
    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("assets/player.png"))
    {
        return std::unexpected("Erreur lors du chargement de la texture");
    }

    playerTexture.setSmooth(true);
    sf::Sprite playerSprite;
    playerSprite.setTextureRect(sf::IntRect(10, 10, 32, 32));
    playerSprite.setTexture(playerTexture);
    playerSprite.setPosition(widthWindow / 2.f, heightWindow / 2.f);
    



    // Création d'une fenêtre "render"
    sf::RenderWindow window(sf::VideoMode(widthWindow, heightWindow), "VampireSurvivor",
                            sf::Style::Default); // Pour un style personnalisé

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
                else if (event.key.code == sf::Keyboard::D)
                {
                    playerSprite.move(10.f, 0.f);
                }
                else if (event.key.code == sf::Keyboard::Q)
                {
                    playerSprite.move(-10.f, 0.f);
                }
                else if (event.key.code == sf::Keyboard::Z)
                {
                    playerSprite.move(0.f, -10.f);
                }
                else if (event.key.code == sf::Keyboard::S)
                {
                    playerSprite.move(0.f, 10.f);
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
