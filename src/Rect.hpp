/******************************************************************************************************
 * @file  Rect.hpp
 * @brief Declaration of the Rect struct
 ******************************************************************************************************/

#pragma once

#include <SDL2/SDL.h>

/**
 * @struct Rect
 * @brief
 */
struct Rect : public SDL_Rect {
    Rect();
    Rect(int x, int y, int w, int h);
};