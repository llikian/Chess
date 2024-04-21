cmake_minimum_required(VERSION 3.25)
project(ChessGame)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL2 REQUIRED)

add_executable(
        ${PROJECT_NAME}

        src/main.cpp

        src/ChessGame.cpp
        src/Color.cpp
        src/Point.cpp
        src/Rect.cpp

        src/graphics.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
        ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
        ${SDL2_LIBRARIES}
        SDL2_image
        SDL2_ttf
)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)