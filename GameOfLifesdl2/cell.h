/**
 * @file cell.h
 * @brief 生命游戏细胞单元定义
 * @details 封装细胞状态和基本操作
 */
#ifndef CELL_H
#define CELL_H

class Cell {
public:
    Cell(int x = 0, int y = 0, bool alive = false);
    bool isAlive() const;
    void setAlive(bool state);
    void calculateNextState(int liveNeighbors);
    void update();
    int getX() const;
    int getY() const;

private:
    bool currentState;  // 当前存活状态
    bool nextState;     // 下一代状态
    int x, y;          // 细胞坐标
};

#endif