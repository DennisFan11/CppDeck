#include "CardZone.cpp"

#pragma once

class HandZone: public CardZone{
    public:
        HandZone(){
            color = BLUE; // 设置手牌区域颜色
            position = {420.0f, 565.0f}; // 手牌区域中心位置
        }
        void addChild(std::shared_ptr<GameObject> child) override{
            children.push_back(child);
            std::dynamic_pointer_cast<Card>(child)-> setState(Card::HAND_ZONE);
        }
};