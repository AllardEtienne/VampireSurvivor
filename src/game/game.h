#pragma once
#include "game_export.h"
#include <stdexcept>
#include <expected>
#include <SFML/Graphics.hpp>
#include "common/common.h"

namespace game::ui
{
int widthWindow = 1000, heightWindow = 800;
int numberOfEnnemies = 5, numberOfProjectile = 10;
GAME_EXPORT __declspec(dllexport) std::expected<int, std::string> runGame();
}; 