#include "CardZone.cpp"

#pragma once

class PublicZone: public CardZone{
    public:
        PublicZone(){
            color = GRAY; // 设置手牌区域颜色
            position = {420.0f, 245.0f}; // 手牌区域中心位置
        }
        void addChild(std::shared_ptr<GameObject> child) override{
            children.push_back(child);
            std::dynamic_pointer_cast<Card>(child)-> setState(Card::PUBLIC_ZONE);
        }
        void draw() override{
            CardZone::draw();
            print_text();
        }
        void print_text(){
            DrawText("Public Zone ", position.x-350, position.y+50, 20, WHITE);
        }
};