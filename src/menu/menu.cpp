#include "menu/menu.h"
#include <SFML/Graphics.hpp>
#include "common/common.h"

namespace menu::ui
{
int widthWindow = 800, heightWindow = 600;

int runMenu()
{
    // Création d'une fenêtre "render"
    sf::RenderWindow window(sf::VideoMode(widthWindow, heightWindow), "VampireSurvivor",
                            sf::Style::Default); // Pour un style personnalisé
    
    common::Button startButton("Start", widthWindow / 2 - 100, heightWindow / 2 - 150);

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
                return 0;
            }

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                {
                    window.close();
                    return 0;
                }
            }

            // D'autres évènements peuvent être gérés ici
        }

        // Effacement de l'ancienne frame (framebuffer)
        window.setVerticalSyncEnabled(true);

        window.clear(sf::Color::Black);

        // Dessiner les éléments du menu
        startButton.drawButton(window);


        // Affiche la nouvelle frame à l'écran
        window.display();
    }
    return 0;
}
}
