#include "common/common.h"

namespace common
{
void center_horizontally(const auto& parent, auto& child)
{
    const auto parentBoundingBox = parent.getLocalBounds();
    const sf::Vector2f parentLocalCenter{parentBoundingBox.left + parentBoundingBox.width * 0.5f,
                                         parentBoundingBox.top + parentBoundingBox.height * 0.5f};

    const sf::Vector2f parentGlobalCenter = parent.getTransform().transformPoint(parentLocalCenter);

    const auto childBoundingBox = child.getLocalBounds();
    const auto childOrigin = child.getOrigin();
    child.setOrigin(childBoundingBox.left + childBoundingBox.width * 0.5f, childOrigin.y);

    const auto childPosition = child.getPosition();
    child.setPosition(parentGlobalCenter.x, childPosition.y);
}

void center_vertically(const auto& parent, auto& child)
{
    const auto parentBoundingBox = parent.getLocalBounds();
    const sf::Vector2f parentLocalCenter{parentBoundingBox.left + parentBoundingBox.width * 0.5f,
                                         parentBoundingBox.top + parentBoundingBox.height * 0.5f};

    const sf::Vector2f parentGlobalCenter = parent.getTransform().transformPoint(parentLocalCenter);

    const auto childBoundingBox = child.getLocalBounds();
    const auto childOrigin = child.getOrigin();
    child.setOrigin(childOrigin.x, childBoundingBox.top + childBoundingBox.height * 0.5f);

    const auto childPosition = child.getPosition();
    child.setPosition(childPosition.x, parentGlobalCenter.y);
}



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
    buttonText.setCharacterSize(50);    // Taille en pixels
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



} // namespace common