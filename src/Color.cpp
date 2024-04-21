/******************************************************************************************************
 * @file  Color.hpp
 * @brief Implementation of the Color class
 ******************************************************************************************************/

#include "Color.hpp"

Color::Color() : SDL_Color({0, 0, 0, 0}) { }

Color::Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a) : SDL_Color({r, g, b, a}) { }

Color::Color(Uint8 r, Uint8 g, Uint8 b) : SDL_Color({r, g, b, 0}) { }

Color::Color(Uint8 value, Uint8 alpha) : SDL_Color({value, value, value, alpha}) { }

Color::Color(Uint8 value) : SDL_Color({value, value, value, 255}) { }

Color::Color(const RGB& color) : SDL_Color({color.r, color.g, color.b, 255}) { }

RGB::RGB() : r(), g(), b() { }

RGB::RGB(Uint8 r, Uint8 g, Uint8 b) : r(r), g(g), b(b) { }

RGB::RGB(Uint8 value) : r(value), g(value), b(value) { }