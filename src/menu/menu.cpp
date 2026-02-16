#include "menu/menu.h"
#include <SFML/Graphics.hpp>
#include "common/common.h"
#include "common/button.h"

namespace menu::ui
{
int widthWindow = 800;
int heightWindow = 600;

int runMenu()
{
    sf::RenderWindow window(sf::VideoMode(widthWindow, heightWindow), "VampireSurvivor", sf::Style::Default);

    window.setVerticalSyncEnabled(true);

    // Création des boutons (position centrée)
    buttons::Button startButton("Start", widthWindow / 2.f - 100.f, heightWindow / 2.f - 80.f);

    buttons::Button quitButton("Quit", widthWindow / 2.f - 100.f, heightWindow / 2.f + 40.f);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                return 0;
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            {
                window.close();
                return 0;
            }

            // Gestion clic souris
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                // START
                if (startButton.getShape().getGlobalBounds().contains(mousePos))
                {
                    return 1; // Lance le jeu
                }

                // QUIT
                if (quitButton.getShape().getGlobalBounds().contains(mousePos))
                {
                    window.close();
                    return 0;
                }
            }
        }

        window.clear(sf::Color::Black);

        startButton.drawButton(window);
        quitButton.drawButton(window);

        window.display();
    }

    return 0;
}
} // namespace menu::ui
