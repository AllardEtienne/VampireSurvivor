add_executable(VampireSurvivor main.cpp "game/game.cpp" "game/game.h" "menu/menu.cpp" "menu/menu.h")



target_link_libraries(VampireSurvivor PUBLIC sfml::sfml
# ou selon les besoins : sfml-system sfml-window sfml-graphics ...
)