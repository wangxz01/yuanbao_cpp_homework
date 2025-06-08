#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <cstdlib>

const int WIDTH = 30;
const int HEIGHT = 20;

class Cell {
public:
    Cell(int x = 0, int y = 0, bool alive = false) : 
        x(x), y(y), currentState(alive), nextState(alive) {}
    
    bool isAlive() const { return currentState; }
    void setAlive(bool state) { currentState = state; }
    void calculateNextState(int liveNeighbors) {
        nextState = (currentState && (liveNeighbors == 2 || liveNeighbors == 3)) || 
                   (!currentState && liveNeighbors == 3);
    }
    void update() { currentState = nextState; }
    int getX() const { return x; }
    int getY() const { return y; }

private:
    bool currentState;
    bool nextState;
    int x, y;
};

class LifeGrid {
public:
    LifeGrid(int width, int height) : width(width), height(height) {
        grid.resize(height);
        for (int y = 0; y < height; ++y) {
            grid[y].resize(width);
            for (int x = 0; x < width; ++x) {
                grid[y][x] = Cell(x, y, false);
            }
        }
    }

    void setInitialAlive(const std::vector<std::pair<int, int>>& aliveCells) {
        for (const auto& coord : aliveCells) {
            int x = coord.first;
            int y = coord.second;
            if (x >= 0 && x < width && y >= 0 && y < height) {
                grid[y][x].setAlive(true);
            }
        }
    }

    void step() {
        // 计算下一代状态
        for (auto& row : grid) {
            for (auto& cell : row) {
                int neighbors = countLiveNeighbors(cell.getX(), cell.getY());
                cell.calculateNextState(neighbors);
            }
        }

        // 应用更新
        for (auto& row : grid) {
            for (auto& cell : row) {
                cell.update();
            }
        }
    }

    void print() const {
        system("clear"); // Linux/macOS清屏，Windows改为"cls"
        
        // 打印顶部边界
        std::cout << "+";
        for (int x = 0; x < width; ++x) {
            std::cout << "---+";
        }
        std::cout << std::endl;

        // 打印每行细胞和网格线
        for (const auto& row : grid) {
            // 细胞行
            std::cout << "|";
            for (const auto& cell : row) {
                std::cout << " " << (cell.isAlive() ? "■" : " ") << " |";
            }
            std::cout << std::endl;

            // 分隔线
            std::cout << "+";
            for (int x = 0; x < width; ++x) {
                std::cout << "---+";
            }
            std::cout << std::endl;
        }
    }

private:
    int countLiveNeighbors(int x, int y) const {
        int count = 0;
        for (int dy = -1; dy <= 1; ++dy) {
            for (int dx = -1; dx <= 1; ++dx) {
                if (dx == 0 && dy == 0) continue;

                // 环形边界处理
                int nx = (x + dx + width) % width;
                int ny = (y + dy + height) % height;

                if (grid[ny][nx].isAlive()) {
                    ++count;
                }
            }
        }
        return count;
    }

    std::vector<std::vector<Cell>> grid;
    int width, height;
};

int main() {
    LifeGrid game(30, 20);
    
    // Gosper滑翔机枪初始配置
    std::vector<std::pair<int, int>> gosper_gun = {
        {1,5},{1,6},{2,5},{2,6},  // 左方块
        {11,5},{11,6},{11,7},      // 右方块
        {12,4},{12,8},
        {13,3},{13,9},
        {14,3},{14,9},             // 枪管
        {15,6},
        {16,4},{16,8},
        {17,5},{17,6},{17,7},
        {18,6},                    // 发射器核心
        {21,3},{21,4},{21,5},
        {22,3},{22,4},{22,5},
        {23,2},{23,6},
        {25,1},{25,2},{25,6},{25,7} // 食者
    };
    game.setInitialAlive(gosper_gun);

    while(true) {
        game.print();
        game.step();
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    return 0;
}