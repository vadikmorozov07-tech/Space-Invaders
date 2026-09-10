cmake_minimum_required(VERSION 3.20)

project(SpaceInvaders VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 3 CONFIG REQUIRED COMPONENTS Graphics Window System)

add_executable(SpaceInvaders
    src/main.cpp
    src/Bullet.cpp
    src/Enemy.cpp
    src/Game.cpp
    src/Menu.cpp
    src/Player.cpp
)

target_include_directories(SpaceInvaders PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(SpaceInvaders PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
)