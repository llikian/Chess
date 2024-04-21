/******************************************************************************************************
 * @file  graphics.cpp
 * @brief 
 ******************************************************************************************************/

#include <stdexcept>
#include "graphics.hpp"

SDL_Texture* createTextureFromSurface(SDL_Renderer* renderer, SDL_Surface* surface, bool freeSurface) {
    if(!surface) {
        throw std::runtime_error{std::string("Surface wasn't defined (nullptr) : ") + SDL_GetError()};
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    if(freeSurface) {
        SDL_FreeSurface(surface);
    }

    if(!texture) {
        SDL_FreeSurface(surface);
        throw std::runtime_error{std::string("Couldn't create texture from surface. ") + SDL_GetError()};
    }

    return texture;
}

void renderText(SDL_Renderer* renderer, TTF_Font* font, int x, int y, const Color& color, const std::string& text) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    if(!surface) {
        throw std::runtime_error{std::string("TTF_RenderText failed. ") + SDL_GetError()};
    }

    SDL_Texture* texture = createTextureFromSurface(renderer, surface, false);

    Rect rect(x, y, surface->w, surface->h);
    SDL_RenderCopy(renderer, texture, nullptr, &rect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void renderText(SDL_Renderer* renderer, TTF_Font* font, int x, int y, const Color& textColor, const Color& background, const std::string& text) {
    SDL_Surface* surface = TTF_RenderText(font, text.c_str(), textColor, background);
    if(!surface) {
        throw std::runtime_error{std::string("TTF_RenderText failed. ") + SDL_GetError()};
    }

    SDL_Texture* texture = createTextureFromSurface(renderer, surface, false);

    Rect rect(x, y, surface->w, surface->h);
    SDL_RenderCopy(renderer, texture, nullptr, &rect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void renderCenteredText(SDL_Renderer* renderer, TTF_Font* font, const Rect& reference, const Color& color, const std::string& text) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    if(!surface) {
        throw std::runtime_error{std::string("TTF_RenderText failed. ") + SDL_GetError()};
    }

    SDL_Texture* texture = createTextureFromSurface(renderer, surface, false);

    Rect rect(reference.x + (reference.w - surface->w) / 2, reference.y + (reference.h - surface->h) / 2, surface->w, surface->h);
    SDL_RenderCopy(renderer, texture, nullptr, &rect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

