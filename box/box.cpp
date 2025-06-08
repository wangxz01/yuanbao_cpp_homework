#include <iostream>
#include <memory>
#include <random>
#include <SDL2/SDL.h>
#include "box.h"

// 定义窗口宽高
const int win_width = 800;
const int win_height = 800;
const int row_cell_num = 160;

// 初始状态活的单元数量
const int life_num = 4800;

// 绘图
void draw(SDL_Renderer* renderer, const std::vector<std::shared_ptr<Cell>>& cells) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    
    for (const auto& cell : cells) {
        if (cell->value == 0) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        } else {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        }
        SDL_FRect rect = {cell->x, cell->y, cell->width, cell->height};
        SDL_RenderFillRectF(renderer, &rect);
    }
}

void run() {
    // 是否完成了初始化绘图
    bool init = false;
    // 控制暂停
    bool change_start = true;
    
    // 初始化SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }
    
    // 初始化容器
    Box box_control(row_cell_num, row_cell_num, win_width, win_height);
    
    // 初始化活的单元
    if (life_num > 0) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> row_dist(0, box_control.row_num - 1);
        std::uniform_int_distribution<> col_dist(0, box_control.col_num - 1);
        
        for (int i = 0; i < life_num; ++i) {
            int row = row_dist(gen);
            int col = col_dist(gen);
            box_control.cells[row][col]->value = 1;
        }
    }
    
    // 创建窗口
    SDL_Window* window = SDL_CreateWindow("Cellular Automata", 
                                         SDL_WINDOWPOS_UNDEFINED, 
                                         SDL_WINDOWPOS_UNDEFINED, 
                                         win_width, 
                                         win_height, 
                                         SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return;
    }
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }
    
    bool quit = false;
    SDL_Event e;
    
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_SPACE) {
                    change_start = !change_start;
                }
            }
        }
        
        if (!init) {
            // 初始化
            init = true;
            auto all_cells = box_control.get_all_cells();
            draw(renderer, all_cells);
        } else {
            if (change_start) {
                auto update_cells = box_control.flush();
                draw(renderer, update_cells);
            }
        }
        
        SDL_RenderPresent(renderer);
        SDL_Delay(1000); // 1秒延迟
    }
    
    // 清理资源
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    run();
    return 0;
}


// g++ box.cpp -o test -I$(brew --prefix sdl2)/include -L$(brew --prefix sdl2)/lib -lSDL2 
//使用SDL进行图形渲染
// 支持160x160的网格
// 随机初始化4800个活细胞
// 按生命游戏规则演化
// 空格键可以暂停/继续模拟
// 每秒更新一次状态