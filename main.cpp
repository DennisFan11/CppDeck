#include <raylib.h>
#include <raymath.h>
#include "include/GameObject.cpp"
#include "include/Sprite.cpp"
#include "include/HandDeckZone.cpp"
#include "include/Card.cpp"

#define WINDOW_WIDTH 1152
#define WINDOW_HEIGHT 648


void game_loop(GameObject &root) { // 游戏循环
	float dt = GetFrameTime(); // 增量时间
	root.update(dt); // 更新所有游戏对象

	BeginDrawing(); // 开始绘制
	ClearBackground(WHITE);
	root.draw(); // 绘制所有游戏对象
	EndDrawing(); // 结束绘制
}

int main() {
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT,"Test");
	SetTraceLogLevel(LOG_WARNING); // 设置日志级别
	SetTargetFPS(60);
	
	GameObject root; //创建根对象
	HandDeckZone* handDeckZone = new HandDeckZone(); //创建手牌区域对象
	root.addChild(std::unique_ptr<GameObject>(handDeckZone)); //将手牌区域对象添加到根对象中

	for(int i = 0; i < 10; i++){
		Card* a_card = new Card("asset/Deck.png", 1, 1); //创建精灵对象
		handDeckZone->addCard(std::unique_ptr<Card>(a_card)); //将精灵对象添加到手牌区域对象中
	}
	
	while (!WindowShouldClose()) {
		game_loop(root); // update all game objects and draw all game objects
	}
	CloseWindow();
}
