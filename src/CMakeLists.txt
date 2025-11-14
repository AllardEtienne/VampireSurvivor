add_executable(VampireSurvivor main.cpp)



target_link_libraries(VampireSurvivor PUBLIC sfml::sfml
# ou selon les besoins : sfml-system sfml-window sfml-graphics ...
)