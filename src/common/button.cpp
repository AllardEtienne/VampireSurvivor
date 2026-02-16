
#include "common/button.h"
#include "common/common.h"

namespace buttons
{

Button::Button(sf::Text initButtonText, sf::Font initFont, sf::RectangleShape initButtonShape)
{
    font = initFont;
    buttonShape = initButtonShape;
    buttonText = initButtonText;
};

Button::Button(sf::String initText, float x, float y)
{
    if (!font.loadFromFile("assets/arial.ttf"))
    {
    }

    buttonShape.setSize(sf::Vector2f(200.f, 100.f));
    buttonShape.setFillColor(sf::Color::White);
    buttonShape.setPosition(x, y);

    buttonText.setFont(font);
    buttonText.setString(initText);
    buttonText.setCharacterSize(50);           // Taille en pixels
    buttonText.setFillColor(sf::Color::Black); // Couleur du texte
    // buttonText.setPosition(300.f, 200.f);

    common::center_horizontally(buttonShape, buttonText);
    common::center_vertically(buttonShape, buttonText);
};

void Button::drawButton(sf::RenderWindow& window)
{
    window.draw(buttonShape);
    window.draw(buttonText);
}

void Button::changeTextColor(sf::Color newColor)
{
    buttonText.setFillColor(newColor);
}

void Button::changeBackgroundColor(sf::Color newColor)
{
    buttonShape.setFillColor(newColor);
}

void Button::changeAllColor(sf::Color newColor)
{
    changeTextColor(newColor);
    changeBackgroundColor(newColor);
}
} // namespace MyNamespace  