#pragma once

#include <SFML/Graphics.hpp>
#include "button_export.h"

namespace buttons
{
class BUTTON_EXPORT Button
{
private:

    sf::Font font;
    sf::Text buttonText;
    sf::RectangleShape buttonShape;

public:

    Button(sf::Text initButtonText, sf::Font initFont, sf::RectangleShape initButtonShape);
    Button(sf::String initText, float x, float y);
    void drawButton(sf::RenderWindow& window);

    void changeTextColor(sf::Color newColor);
    void changeBackgroundColor(sf::Color newColor);
    void changeAllColor(sf::Color newColor);

    public:
    const sf::RectangleShape& getShape() const
    {
        return buttonShape;
    }
};
};
