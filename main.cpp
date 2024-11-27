#include <raylib.h>
#include <raymath.h>
#include "GameObject.cpp"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600


void game_loop(){
	float dt = GetFrameTime(); // 增量时间
}


int main() {
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT,"Test");
	SetTraceLogLevel(LOG_WARNING);
	SetTargetFPS(60);
	
	//创建一个40*40,透明背景的图片
	Image img=GenImageColor(40,40,BLANK);
	//在图片中绘制一个以(20,20)为圆心，半径为20的红色圆
	ImageDrawCircle(&img,20,20,19,RED);
	
	Texture texture = LoadTextureFromImage(img);
	
	UnloadImage(img); //不再需要使用img，释放掉
	int x=0;
	int y=0;
	int step=5;
	while (!WindowShouldClose()) {
		game_loop() // update all game objects
		//更新圆的位置（texture显示的位置）
		x+=5;
		if (x>WINDOW_WIDTH-texture.width) {
			y+=texture.height;
			x=0;
			if (y>WINDOW_HEIGHT-texture.height) {
				y=0;
			}
		}
		//绘制帧
		BeginDrawing();
		ClearBackground(WHITE);
		DrawTexture(texture,x,y,WHITE);
		EndDrawing();
	}
	UnloadTexture(texture); //释放texture对象
	CloseWindow();
}
