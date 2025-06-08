/**
 * @file lifegrid.cpp
 * @brief 网格系统实现
 */
#include "lifegrid.h"
#include <iostream>
#include <cstdlib>

LifeGrid::LifeGrid(int width, int height) : width(width), height(height) {
    grid.resize(height);
    for (int y = 0; y < height; ++y) {
        grid[y].resize(width);
        for (int x = 0; x < width; ++x) {
            grid[y][x] = Cell(x, y, false);
        }
    }
}

void LifeGrid::setInitialAlive(const std::vector<std::pair<int, int>>& aliveCells) {
    for (const auto& coord : aliveCells) {
        int x = coord.first;
        int y = coord.second;
        if (x >= 0 && x < width && y >= 0 && y < height) {
            grid[y][x].setAlive(true);
        }
    }
}

/**
 * @brief 打印带网格线的世界状态
 */
void LifeGrid::print() const {
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

int LifeGrid::countLiveNeighbors(int x, int y) const {
    int count = 0;
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            if (dx == 0 && dy == 0) continue;
            int nx = (x + dx + width) % width; // 环形边界
            int ny = (y + dy + height) % height;
            if (grid[ny][nx].isAlive()) ++count;
        }
    }
    return count;
}

void LifeGrid::step() {
    // 计算下一代状态
    for (auto& row : grid) {
        for (auto& cell : row) {
            cell.calculateNextState(countLiveNeighbors(cell.getX(), cell.getY()));
        }
    }

    // 应用更新
    for (auto& row : grid) {
        for (auto& cell : row) {
            cell.update();
        }
    }
}