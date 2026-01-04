#pragma once
#include "game_export.h"
#include <stdexcept>
#include <expected>
#include <SFML/Graphics.hpp>

namespace game::ui
{
GAME_EXPORT __declspec(dllexport) std::expected<int, std::string> runGame();

GAME_EXPORT void moveUp(sf::Sprite& sprite);
GAME_EXPORT void moveDown(sf::Sprite& sprite);
GAME_EXPORT void moveLeft(sf::Sprite& sprite);
GAME_EXPORT void moveRight(sf::Sprite& sprite);
GAME_EXPORT void shootProjectile(std::vector<sf::CircleShape>& projectile, int count, sf::Color colors);
GAME_EXPORT void moveTo(std::vector<sf::CircleShape>& projectiles, float speed);


}; 