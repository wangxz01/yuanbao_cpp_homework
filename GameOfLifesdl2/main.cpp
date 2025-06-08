/**
 * @file main.cpp
 * @brief 生命游戏主程序
 */

#include "lifegrid.h"
#include "sdl_renderer.h"
#include <chrono>
#include <thread>

int main() {
    const int WIDTH = 60, HEIGHT = 40;
    LifeGrid game(WIDTH, HEIGHT);
    

    // game.setInitialAlive({{1,0}, {2,1}, {0,2}, {1,2}, {2,2}}); //滑翔机

   
    // std::vector<std::pair<int, int>> gosper_gun = { // Gosper滑翔机枪
    //     {1,5},{1,6},{2,5},{2,6},  // 左方块
    //     {11,5},{11,6},{11,7},      // 右方块
    //     {12,4},{12,8},
    //     {13,3},{13,9},
    //     {14,3},{14,9},             // 枪管
    //     {15,6},
    //     {16,4},{16,8},
    //     {17,5},{17,6},{17,7},
    //     {18,6},                    // 发射器核心
    //     {21,3},{21,4},{21,5},
    //     {22,3},{22,4},{22,5},
    //     {23,2},{23,6},
    //     {25,1},{25,2},{25,6},{25,7} // 食者
    // };
    // game.setInitialAlive(gosper_gun);

	std::vector<std::pair<int, int>> r_pentomino = {//R-pentomino混沌演化
    {10,10}, {11,10}, {12,10},
    {10,11}, {11,9}
	};
	game.setInitialAlive(r_pentomino);


	// std::vector<std::pair<int, int>> pulsar = {//脉冲星（振荡器）
	// 	{2,4},{2,5},{2,6},{2,10},{2,11},{2,12},
	// 	{4,2},{4,7},{4,9},{4,14},
	// 	{5,2},{5,7},{5,9},{5,14},
	// 	{6,2},{6,7},{6,9},{6,14},
	// 	{7,4},{7,5},{7,6},{7,10},{7,11},{7,12},
	// 	{9,4},{9,5},{9,6},{9,10},{9,11},{9,12},
	// 	{10,2},{10,7},{10,9},{10,14},
	// 	{11,2},{11,7},{11,9},{11,14},
	// 	{12,2},{12,7},{12,9},{12,14},
	// 	{14,4},{14,5},{14,6},{14,10},{14,11},{14,12}
	// }; 
	// game.setInitialAlive(pulsar);


    SDLRenderer renderer(WIDTH, HEIGHT);
    
    while(renderer.handleEvents()) {
        renderer.render(game);
        game.step();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return 0;
    // while(true) {
    //     game.print();
    //     game.step();
    //     std::this_thread::sleep_for(std::chrono::milliseconds(200));
    // }
    // return 0;
}
