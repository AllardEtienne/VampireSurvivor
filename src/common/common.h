#pragma once

#include <SFML/Graphics.hpp>
#include "common_export.h"

namespace common
{
COMMON_EXPORT void center_horizontally(const auto& parent, auto& child);

COMMON_EXPORT void center_vertically(const auto& parent, auto& child);

COMMON_EXPORT float random_float(float min, float max);

class COMMON_EXPORT Ennemy
{
    private:
    float y;
    float x;
    float yDestination;
    float xDestination;
    sf::CircleShape shape;

    public:
    Ennemy(float initY, float initX, float initYDestination, float initXDestination, sf::CircleShape initShape);
    Ennemy();
    void drawEnnemy(sf::RenderWindow& window) const;
    void changeDestination(float newYDestination, float newXDestination);
    void moveTowardsDestination(float speed);
    bool destination_reached();
    sf::Vector2f getPosition();
    


};

class COMMON_EXPORT Button
{
private:
    sf::Font font;
    sf::Text buttonText;
    sf::RectangleShape buttonShape;

public:
    Button(sf::Text initButtonText, sf::Font initFont,
           sf::RectangleShape initButtonShape);
    Button(sf::String initText, float x, float y);
    void drawButton(sf::RenderWindow& window);


    void changeTextColor(sf::Color newColor);
    void changeBackgroundColor(sf::Color newColor);
    void changeAllColor(sf::Color newColor);
};


}; // namespace common