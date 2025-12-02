#pragma once
#include "game_export.h"
#include <stdexcept>
#include <expected>

namespace game::ui
{
GAME_EXPORT __declspec(dllexport) std::expected<int, std::string> runGame();


}; 