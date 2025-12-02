include(GenerateExportHeader)


# Create a library target called 'common'
add_library(common SHARED
    common/common.cpp
)

target_include_directories(common 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(common 
            PUBLIC sfml::sfml
)


set_target_properties(common PROPERTIES DEFINE_SYMBOL COMMON_EXPORTS)
generate_export_header(common
    BASE_NAME        COMMON
    EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/common_export.h
)




# Create a library target called 'menu_ui'
add_library(menu_ui SHARED
    menu/menu.cpp
)

target_include_directories(menu_ui 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(menu_ui 
            PUBLIC sfml::sfml common
)

set_target_properties(menu_ui PROPERTIES DEFINE_SYMBOL MENU_WINDOW_EXPORTS)
generate_export_header(menu_ui
    BASE_NAME        MENU
    EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/menu_export.h
)

# Create a library target called 'game_ui'
add_library(game_ui SHARED
    game/game.cpp
)

target_include_directories(game_ui 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(game_ui 
            PUBLIC sfml::sfml common
)

set_target_properties(game_ui PROPERTIES DEFINE_SYMBOL GAME_WINDOW_EXPORTS)
generate_export_header(game_ui
    BASE_NAME        GAME
    EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/game_export.h
)



add_executable(VampireSurvivor main.cpp)

target_link_libraries(VampireSurvivor PUBLIC sfml::sfml menu_ui game_ui
# ou selon les besoins : sfml-system sfml-window sfml-graphics ...
)