#include <SDL2/SDL.h>
#include <vector>
#include <random>
#include <chrono>
#include <thread>

const int WIDTH = 300, HEIGHT = 300;
const float TREE_GROWTH_PROB = 0.01f;   
// 树木生长概率 p
const float LIGHTNING_PROB = 6e-5f;     
// 闪电概率 f
const float WIND_INFLUENCE = 0.8f;       
// 风的影响概率 w

enum CellState { EMPTY = 0, TREE = 1, BURNING = 2 };

// 初始化随机数生成器
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<float> dis(0.0f, 1.0f);

// 森林网格（带边界）
std::vector<std::vector<CellState>> forest(WIDTH + 2, std::vector<CellState>(HEIGHT + 2, EMPTY));

// 初始化森林（中心点火）
void initForest() {
    for (int i = 1; i <= WIDTH; ++i) {
        for (int j = 1; j <= HEIGHT; ++j) {
            forest[i][j] = TREE;
        }
    }
    forest[WIDTH/2][HEIGHT/2] = BURNING;  // 中心点火
}

// 更新森林状态
void updateForest() {
    auto newForest = forest;

    for (int i = 1; i <= WIDTH; ++i) {
        for (int j = 1; j <= HEIGHT; ++j) {
            // 规则1：燃烧的树变为空地
            if (forest[i][j] == BURNING) {
                newForest[i][j] = EMPTY;
            }
            // 规则2：检查邻居是否着火
            else if (forest[i][j] == TREE) {
                int burningNeighbors = 0;
                for (int di = -1; di <= 1; ++di) {
                    for (int dj = -1; dj <= 1; ++dj) {
                        if (di == 0 && dj == 0) continue;
                        if (forest[i + di][j + dj] == BURNING) {
                            burningNeighbors++;
                        }
                    }
                }
                if (burningNeighbors > 0) {
                    newForest[i][j] = BURNING;
                }
                // 规则3：风的影响（左侧更容易着火）
                else if (forest[i-1][j] == BURNING && dis(gen) < WIND_INFLUENCE) {
                    newForest[i][j] = BURNING;
                }
                // 规则4：闪电点燃
                else if (dis(gen) < LIGHTNING_PROB) {
                    newForest[i][j] = BURNING;
                }
            }
            // 规则5：空地随机生长树木
            else if (forest[i][j] == EMPTY && dis(gen) < TREE_GROWTH_PROB) {
                newForest[i][j] = TREE;
            }
        }
    }
    forest = newForest;
}

// 渲染森林到SDL窗口
void renderForest(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // 黑色背景
    SDL_RenderClear(renderer);

    for (int i = 1; i <= WIDTH; ++i) {
        for (int j = 1; j <= HEIGHT; ++j) {
            SDL_Rect rect = { (i-1)*2, (j-1)*2, 2, 2 };  // 每个单元格2x2像素
            if (forest[i][j] == TREE) {
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  // 绿色树木
            } else if (forest[i][j] == BURNING) {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // 红色燃烧
            } else {
                continue;  // 空地不渲染（保持黑色）
            }
            SDL_RenderFillRect(renderer, &rect);
        }
    }
    SDL_RenderPresent(renderer);
}

int main() {
    // 初始化SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL初始化失败: %s", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("森林火灾模拟 (macOS)", 
                                        SDL_WINDOWPOS_CENTERED, 
                                        SDL_WINDOWPOS_CENTERED,
                                        WIDTH*2, HEIGHT*2, 
                                        SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_Log("窗口创建失败: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_Log("渲染器创建失败: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    initForest();

    // 主循环
    bool quit = false;
    SDL_Event event;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        updateForest();
        renderForest(renderer);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));  // 控制速度
    }

    // 清理资源
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

//g++ f.cpp -o test -I$(brew --prefix sdl2)/include -L$(brew --prefix sdl2)/lib -lSDL2 