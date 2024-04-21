cmake_minimum_required(VERSION 3.25)

set(CMAKE_CXX_STANDARD 17)

project(ChessGame)

find_package(SDL2)

set(CMAKE_CXX_STANDARD 20)

add_executable(
        ${PROJECT_NAME}

        src/main.cpp

        src/ChessGame.cpp
        src/Color.cpp
        src/Point.cpp
        src/Rect.cpp

        src/graphics.cpp
)

target_link_libraries(${PROJECT_NAME} SDL2 SDL2main SDL2_image SDL2_ttf)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)