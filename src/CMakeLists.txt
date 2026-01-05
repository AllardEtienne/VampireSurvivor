include(GenerateExportHeader)

add_library(ecs SHARED
    "ecs/internal/component_manager.cpp"
    "ecs/internal/entity_manager.cpp"
    "ecs/internal/system_manager.cpp")

add_library(lib::ecs ALIAS ecs)

target_include_directories(ecs
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
)

generate_export_header(ecs
    BASE_NAME        ECS
    EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/ecs/export.h
)

add_executable(ecs_tests
    ecs/tests/core_suitetests.cpp)

target_link_libraries(ecs_tests
    PRIVATE
        lib::ecs
        GTest::gtest
        GTest::gtest_main
)

add_executable(ecs_tests_new
    ecs/tests/core_suitetests_new.cpp)

target_link_libraries(ecs_tests_new
    PRIVATE
        lib::ecs
        GTest::gtest
        GTest::gtest_main
        sfml::sfml
)

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
            PUBLIC sfml::sfml common lib::ecs
)

set_target_properties(game_ui PROPERTIES DEFINE_SYMBOL GAME_WINDOW_EXPORTS)
generate_export_header(game_ui
    BASE_NAME        GAME
    EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/game_export.h
)




add_executable(VampireSurvivor main.cpp)

target_link_libraries(VampireSurvivor PUBLIC sfml::sfml menu_ui game_ui lib::ecs
# ou selon les besoins : sfml-system sfml-window sfml-graphics ...
)