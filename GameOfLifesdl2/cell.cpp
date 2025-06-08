/**
 * @file cell.cpp
 * @brief 细胞类方法实现
 */
#include "cell.h"

Cell::Cell(int x, int y, bool alive) : 
    x(x), y(y), currentState(alive), nextState(alive) {}

bool Cell::isAlive() const { return currentState; }
void Cell::setAlive(bool state) { currentState = state; }

/**
 * @brief 计算下一代状态
 * @param liveNeighbors 周围存活细胞数
 */
void Cell::calculateNextState(int liveNeighbors) {
    nextState = (currentState && (liveNeighbors == 2 || liveNeighbors == 3)) || 
               (!currentState && liveNeighbors == 3);
}

void Cell::update() { currentState = nextState; }
int Cell::getX() const { return x; }
int Cell::getY() const { return y; }