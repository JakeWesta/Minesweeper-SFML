

cmake_minimum_required(VERSION 3.2)
project(minesweeper)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(minesweeper src/main.cpp
        src/Welcome.cpp
        src/Welcome.h
        src/board.cpp
        src/board.h
        src/tile.cpp
        src/tile.h
        src/TextureManager.cpp
        src/leaderboard.cpp
        src/leaderboard.h
        src/players.cpp
        src/players.h
        src/Difficulty.cpp
        src/Difficulty.h
)
set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR C:/SFML/lib/cmake/SFML)

find_package(SFML 2.5.1 COMPONENTS system window graphics audio REQUIRED)
target_link_libraries(minesweeper sfml-system sfml-window sfml-graphics sfml-audio)