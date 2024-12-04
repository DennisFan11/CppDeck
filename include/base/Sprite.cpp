#include <raylib.h>
#include "GameObject.cpp"
// #include "VectorUtils.cpp"


#pragma once
class Sprite : public GameObject
{
    private:
        Texture2D texture;

        Vector2 size; // 精灵的大小
        float rotation = 0.0f; // 精灵的旋转角度
        Vector2 origin = {1.0f, 1.0f}; // 精灵的原点
        
        Color tint = WHITE; // 精灵的颜色
    public:
        Sprite(const char* path):GameObject(){
            texture = LoadTexture(path);
            size = {(float)texture.width, (float)texture.height};
        }
        void draw() override{
            GameObject::draw();
            Rectangle sourceRec = {0.0f, 0.0f, size.x, size.y}; // 设置裁剪矩形
            Vector2 LT = position - size/2.0f;
            Rectangle destRec = {LT.x, LT.y, size.x, size.y}; // 设置目标矩形
            DrawTexturePro(texture, sourceRec, destRec, origin, rotation, tint);
        }
        void update(float delta) override{
            GameObject::update(delta);
        }

        void setSize(Vector2 size){
            this->size = size;
        }
        void setOrigin(Vector2 origin){
            this->origin = origin;
        }
        void setPosition(Vector2 position){
            this->position = position;
        }
        void setRotation(float rotation){
            this->rotation = rotation;
        }
        void setTint(Color tint){ // 设置精灵的颜色
            this->tint = tint;
        }
};