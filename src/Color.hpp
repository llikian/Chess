/******************************************************************************************************
 * @file  Color.hpp
 * @brief Definition of the Color struct
 ******************************************************************************************************/

#pragma once

#include <SDL2/SDL.h>

/**
 * @struct
 * @brief
 */
struct RGB {
    Uint8 r, g, b;

    RGB();
    RGB(Uint8 r, Uint8 g, Uint8 b);
    RGB(Uint8 value);
};

/**
 * @struct Color
 * @brief
 */
struct Color : public SDL_Color {
    Color();
    Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    Color(Uint8 r, Uint8 g, Uint8 b);
    Color(Uint8 value, Uint8 alpha);
    Color(Uint8 value);
    Color(const RGB& color);
};