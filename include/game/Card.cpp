// #include <raylib.h>
// #include <vector>
// #include <memory>
#include "../base/Sprite.cpp"
#include "../base/GameObject.cpp"


#pragma once
class Card: public GameObject{
    private:
        int number;
        int suit;
        bool isFaceUp;
        bool topCard = false; // 是否是最上面的一张牌

        Vector2 target_position;
        const Vector2 cardSize = {90.0f, 128.0f};
        std::unique_ptr<Sprite> backgroundSprite;
    public:
        Card(const char* path, int number, int suit){
            backgroundSprite = std::make_unique<Sprite>(path);
            this->number = number;
            this->suit = suit;
            this->isFaceUp = false;
        }

        void draw() override{
            GameObject::draw();
            backgroundSprite->setPosition(position); // 设置精灵的位置
            backgroundSprite->setTint(topCard ? GRAY : WHITE); // 设置精灵的颜色
            backgroundSprite->draw();
            if(isFaceUp){
                
            }
        }
        void update(float delta) override{
            
            

            if (Vector2Distance(position, target_position) > 1.0f){
                position += (target_position - position) * 4.5f * delta;
            }else{
                position = target_position;
            }
            GameObject::update(delta);
            backgroundSprite->update(delta);
        }
        void flip(){
            isFaceUp = !isFaceUp;
        }
        int getNumber(){
            return number;
        }
        int getSuit(){
            return suit;
        }
        bool getIsFaceUp(){
            return isFaceUp;
        }
        void setTopCard(bool topCard){
            this->topCard = topCard;
        }
        bool getTopCard(){
            return topCard;
        }
        bool isMouseInside(){
            Vector2 LT = position - cardSize/2.0f;
            Vector2 RD = position + cardSize/2.0f;
            Rectangle zone = {LT.x, LT.y, cardSize.x, cardSize.y};
            return CheckCollisionPointRec(GetMousePosition(), zone);
        }
        void setTargetPosition(Vector2 target_position){
            this->target_position = target_position;
        }

};