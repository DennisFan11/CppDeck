#include "CardZone.cpp"

#pragma once

class TrashCanZone: public CardZone{
    public:
        TrashCanZone(){
            color = BLACK; // 设置手牌区域颜色
            position = {915.0f, 565.0f}; // 手牌区域中心位置
            size = {150.0f, 150.0f}; // 手牌区域大小
        }
        void addChild(std::shared_ptr<GameObject> child) override{ // 

        }
};