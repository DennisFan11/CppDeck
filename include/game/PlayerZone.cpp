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
};