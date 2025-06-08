/**
 * @file sdl_renderer.h
 * @brief SDL2图形渲染封装
 */
#ifndef SDL_RENDERER_H
#define SDL_RENDERER_H

#include <SDL2/SDL.h>
#include "lifegrid.h"

class SDLRenderer {
public:
    SDLRenderer(int width, int height, int cellSize = 20);
    ~SDLRenderer();
    void render(const LifeGrid& grid);
    bool handleEvents();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    int cellSize;
    int width, height;
};

#endif