#include "common/common.h"
#include <random>

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

float random_float(float min, float max)
{
    static std::mt19937 generator{std::random_device{}()};
    std::uniform_real_distribution<float> distribution(min, max);
    return distribution(generator);
}


Ennemy::Ennemy(float initY, float initX, float initYDestination, float initXDestination, sf::CircleShape initShape)
{
    y = initY;
    x = initX;
    yDestination = initYDestination;
    xDestination = initXDestination;
    shape = initShape;
}

Ennemy::Ennemy()
{
    y = 0.f;
    x = 0.f;
    yDestination = 0.f;
    xDestination = 0.f;
    shape.setRadius(20.f);
    shape.setFillColor(sf::Color::Blue);
    shape.setPosition(x, y);
};

void Ennemy::drawEnnemy(sf::RenderWindow& window) const
{
    window.draw(shape);
}

void Ennemy::changeDestination(float newYDestination, float newXDestination)
{
    yDestination = newYDestination;
    xDestination = newXDestination;
}

void Ennemy::moveTowardsDestination(float speed)
{
    sf::Vector2f position = shape.getPosition();
    sf::Vector2f direction = sf::Vector2f(xDestination, yDestination) - position;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length != 0)
    {
        direction /= length; // Normalisation
        shape.move(direction * speed);
    }
}

sf::Vector2f Ennemy::getPosition()
{
    return shape.getPosition();
}

bool Ennemy::destination_reached()
{
    sf::Vector2f position = shape.getPosition();
    sf::Vector2f destination = sf::Vector2f(xDestination, yDestination);
    float distance = std::sqrt(std::pow(destination.x - position.x, 2) + std::pow(destination.y - position.y, 2));
    return distance < 1.0f; // Considérer comme atteint si la distance est inférieure à 1 pixel
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