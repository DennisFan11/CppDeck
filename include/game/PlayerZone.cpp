#include "CardZone.cpp"

#pragma once

class PlayerZone: public CardZone{
    public:
        PlayerZone(){
            color = BLUE; // 设置手牌区域颜色
            position = {420.0f, 405.0f}; // 手牌区域中心位置
        }
        void addChild(std::shared_ptr<GameObject> child) override{
            children.push_back(child);
            std::dynamic_pointer_cast<Card>(child)-> setState(Card::PLAYER_ZONE);
        }
        void draw() override{
            CardZone::draw();
            print_text();
        }
        int point = 0;
        void print_text(){
            DrawText(("Player Zone (point : " + std::to_string(point) + ")").c_str(), position.x-350, position.y+50, 20, WHITE);
        }
};