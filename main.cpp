#include <raylib.h>
#include <raymath.h>



#define WINDOW_WIDTH 1152
#define WINDOW_HEIGHT 648



#include "include/game/_GameBoard.cpp"
std::shared_ptr<_GameBoard> game_board;



void game_loop(GameObject &root) { // 游戏循环
	float dt = GetFrameTime(); // 增量时间
	root.r_update(dt); // 更新所有游戏对象
	root.r_input(); // 输入所有游戏对象

	BeginDrawing(); // 开始绘制
	ClearBackground(WHITE);
	root.r_draw(); // 绘制所有游戏对象
	EndDrawing(); // 结束绘制
}


int main() {
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT,"Test");
	SetTraceLogLevel(LOG_NONE); // 设置日志级别
	SetTargetFPS(60);
	
	GameObject root; //创建根对象
	game_board = std::make_shared<_GameBoard>(); //创建手牌区域对象
	root.addChild(game_board); //将手牌区域对象添加到根对象中
	
	while (!WindowShouldClose()) {
		game_loop(root); // update all game objects and draw all game objects
	}
	CloseWindow();
}
