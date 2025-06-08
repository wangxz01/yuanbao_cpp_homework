#include "sdl_renderer.h"
#include <stdexcept>

SDLRenderer::SDLRenderer(int w, int h, int cs) : 
    width(w), height(h), cellSize(cs) {
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
        throw std::runtime_error(SDL_GetError());

    window = SDL_CreateWindow("Conway's Game of Life",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width * cellSize, height * cellSize, 
        SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(window, -1, 
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

SDLRenderer::~SDLRenderer() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void SDLRenderer::render(const LifeGrid& grid) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // 绘制网格线
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    for(int x = 0; x <= width; ++x)
        SDL_RenderDrawLine(renderer, x*cellSize, 0, x*cellSize, height*cellSize);
    for(int y = 0; y <= height; ++y)
        SDL_RenderDrawLine(renderer, 0, y*cellSize, width*cellSize, y*cellSize);

    // 绘制活细胞
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    for(int y = 0; y < height; ++y) {
        for(int x = 0; x < width; ++x) {
            if(grid.getCellState(x, y)) {
                SDL_Rect rect = {x*cellSize+1, y*cellSize+1, cellSize-1, cellSize-1};
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
    SDL_RenderPresent(renderer);
}

bool SDLRenderer::handleEvents() {
    SDL_Event e;
    while(SDL_PollEvent(&e)) {
        if(e.type == SDL_QUIT) return false;
        if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) return false;
    }
    return true;
}