#include "CardZone.cpp"

#pragma once

class EnemyZone: public CardZone{
    public:
        EnemyZone(){
            color = RED; // 设置手牌区域颜色
            position = {420.0f, 85.0f}; // 手牌区域中心位置
        }
        void addChild(std::shared_ptr<GameObject> child) override{
            children.push_back(child);
            std::dynamic_pointer_cast<Card>(child)-> setState(Card::ENEMY_ZONE);
        }
};