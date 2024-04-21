/******************************************************************************************************
 * @file  graphics.hpp
 * @brief 
 ******************************************************************************************************/

#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

#include "Color.hpp"
#include "Rect.hpp"

/**
 * @brief
 *
 * @param renderer
 * @param surface
 * @param freeSurface
 *
 * @return
 */
SDL_Texture* createTextureFromSurface(SDL_Renderer* renderer, SDL_Surface* surface, bool freeSurface = true);

/**
 * @brief
 *
 * @param renderer
 * @param font
 * @param x
 * @param y
 * @param color
 * @param text
 */
void renderText(SDL_Renderer* renderer, TTF_Font* font, int x, int y, const Color& color, const std::string& text);

/**
 * @brief
 *
 * @param renderer
 * @param font
 * @param x
 * @param y
 * @param textColor
 * @param text
 */
void renderText(SDL_Renderer* renderer, TTF_Font* font, int x, int y, const Color& textColor, const Color& background, const std::string& text);

/**
 * @brief
 *
 * @param renderer
 * @param font
 * @param reference
 * @param color
 * @param text
 */
void renderCenteredText(SDL_Renderer* renderer, TTF_Font* font, const Rect& reference, const Color& color, const std::string& text);