#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
using namespace std;

const int ROWS = 20, COLS = 50;

void printGrid(const vector<vector<bool>>& grid) {
    system("clear"); // macOS清屏命令
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) 
            cout << (grid[i][j] ? "■" : " ");
        cout << endl;
    }
}

int countNeighbors(const vector<vector<bool>>& grid, int x, int y) {
    int count = 0;
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (i == 0 && j == 0) continue;
            // 环形边界处理
            int nx = (x + i + ROWS) % ROWS;
            int ny = (y + j + COLS) % COLS;
            count += grid[nx][ny];
        }
    }
    return count;
}

void updateGrid(vector<vector<bool>>& grid) {
    vector<vector<bool>> newGrid = grid;
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            int neighbors = countNeighbors(grid, i, j);
            if (grid[i][j]) 
                newGrid[i][j] = (neighbors == 2 || neighbors == 3);
            else 
                newGrid[i][j] = (neighbors == 3);
        }
    }
    grid = newGrid;
}

int main() {
    vector<vector<bool>> grid(ROWS, vector<bool>(COLS, false));
    
    // 初始化滑翔机模式
    grid[1][2] = grid[2][3] = grid[3][1] = grid[3][2] = grid[3][3] = true;

    while (true) {
        printGrid(grid);
        updateGrid(grid);
        this_thread::sleep_for(chrono::milliseconds(200));
    }
}