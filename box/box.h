//头文件保护
#ifndef BOX_H
#define BOX_H


#include <vector>
#include <memory>

class Cell {//位置大小状态
public:
    // 单元格左上角坐标 x 值
    float x;
    // 单元格左上角坐标 y 值
    float y;
    // 单元格高度
    float height;
    // 单元格宽度
    float width;
    // 单元格值，默认为 0
    int value;

    Cell(float x, float y, float width, float height)
        : x(x), y(y), width(width), height(height), value(0) {}
};

class Box {//窗口尺寸、行列数和单元格二维数组
public:
    // 窗口宽
    int window_width;
    // 窗口高
    int window_height;
    // 行数
    int row_num;
    // 列数
    int col_num;
    std::vector<std::vector<std::shared_ptr<Cell>>> cells;

    Box(int row_num, int col_num, int win_width, int win_height)//初始化网格，计算每个单元格的位置和大小。
        : row_num(row_num), col_num(col_num), window_width(win_width), window_height(win_height) {
        cells.resize(row_num);
        for (int row = 0; row < row_num; ++row) {
            cells[row].resize(col_num);
            for (int col = 0; col < col_num; ++col) {
                cells[row][col] = std::make_shared<Cell>(
                    row * (static_cast<float>(window_width) / row_num),
                    col * (static_cast<float>(window_height) / col_num),
                    static_cast<float>(window_width) / row_num,
                    static_cast<float>(window_height) / col_num
                );
            }
        }
    }

    // 获取指定的 cell
    std::shared_ptr<Cell> get_cell(int row, int col) {
        if (row < 0) {//行号越界
            row = (row % row_num) + row_num - 1;//负数环绕
        }
        if (row >= row_num) {
            row = row % row_num;//正数超界环绕
        }
        
        if (col < 0) {//列号越界
            col = (col % col_num) + col_num - 1;
        }
        if (col >= col_num) {
            col = col % col_num;//防止越界
        }
        return cells[row][col];
    }

    // 获取所有单元
    std::vector<std::shared_ptr<Cell>> get_all_cells() {
        std::vector<std::shared_ptr<Cell>> all_cells;
        for (auto& row : cells) {
            for (auto& cell : row) {
                all_cells.push_back(cell);
            }
        }
        return all_cells;
    }

    // 根据自动机规则，刷新状态
    // 返回状态变更了的单元
    std::vector<std::shared_ptr<Cell>> flush() {//生命游戏规则的实现
        // 需要变更状态的单元索引
        std::vector<std::pair<int, int>> update_cells_index;
        
        // 遍历所有单元，更新值
        for (int row = 0; row < row_num; ++row) {
            for (int col = 0; col < col_num; ++col) {
                // 读取周围8个单元值
                auto cur = get_cell(row, col);
                auto top = get_cell(row - 1, col);
                auto left = get_cell(row, col - 1);
                auto bottom = get_cell(row + 1, col);
                auto right = get_cell(row, col + 1);
                auto top_left = get_cell(row - 1, col - 1);
                auto bottom_left = get_cell(row + 1, col - 1);
                auto top_right = get_cell(row - 1, col + 1);
                auto bottom_right = get_cell(row + 1, col + 1);
                
                // 存活的节点数量
                int surviving_count = top->value
                                   + left->value
                                   + bottom->value
                                   + right->value
                                   + top_left->value
                                   + bottom_left->value
                                   + top_right->value
                                   + bottom_right->value;
                
                // 根据生命游戏规则，更新状态
                if (cur->value == 1) {
                    if (surviving_count < 2 || surviving_count > 3) {
                        update_cells_index.emplace_back(row, col);
                    }
                } else {
                    if (surviving_count == 3) {
                        update_cells_index.emplace_back(row, col);
                    }
                }
            }
        }
        
        std::vector<std::shared_ptr<Cell>> update_cells;
        // 执行状态值变更
        for (auto& index : update_cells_index) {
            int row = index.first;
            int col = index.second;
            if (cells[row][col]->value == 1) {
                cells[row][col]->value = 0;
            } else {
                cells[row][col]->value = 1;
            }
            update_cells.push_back(cells[row][col]);
        }
        return update_cells;
    }
};

#endif // BOX_H