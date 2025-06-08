/**
 * @file lifegrid.h
 * @brief 生命游戏网格系统
 * @details 管理细胞集合和游戏规则
 */
#ifndef LIFEGRID_H
#define LIFEGRID_H

#include "cell.h"
#include <vector>

class LifeGrid {
public:
    LifeGrid(int width, int height);
    void setInitialAlive(const std::vector<std::pair<int, int>>& aliveCells);
    void step();
    void print() const;  // 修改为网格化输出

private:
    int countLiveNeighbors(int x, int y) const;
    std::vector<std::vector<Cell>> grid;
    int width, height;
};

#endif