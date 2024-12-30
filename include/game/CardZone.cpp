#include "../base/GameObject.cpp"
#include "Card.cpp"
// #include <GameObject.cpp>

#pragma once

class CardZone:public GameObject {
    protected:
        Vector2 position = {0.0f, 0.0f}; // 手牌区域中心位置
        Vector2 size = {760.0f, 150.0f}; // 手牌区域大小
        Color color = RED; // 手牌区域颜色

        void draw_self(){ // 绘制手牌区域
            Vector2 pos = position - size/2.0f; // 计算手牌区域左上角位置
            DrawRectangleV(pos, size, color); 
        }
        void update_card_pos(float delta){ // 更新手牌位置
            const float cardInterval = 60.0f;
            Vector2 cardSize = {90.0f, 128.0f};
            Vector2 cardPos = position - Vector2{cardInterval * (float)(children.size()/2.0f), 0.0f}; // 计算第一张手牌的位置

            for (int i = 0; i < children.size(); i++){
                auto card = std::dynamic_pointer_cast<Card>(children[i]);
                card->setTargetPosition(cardPos);
                cardPos.x += cardInterval; // 更新下一张手牌的位置
            }
        }


        
    public:

        std::string to_string(){
            std::string res = "";
            for (auto& child : children){
                res += std::dynamic_pointer_cast<Card>(child)->to_string() + " ";
            }
            return res;
        }
        CardZone() = default;

        void draw() override{ // 绘制手牌
            draw_self(); // 绘制手牌区域
            GameObject::draw();
            
        }
        void update(float delta) override{ // 更新手牌
            update_card_pos(delta);
            GameObject::update(delta);
        }
        Rectangle getZone(){ // 获取手牌区域
            return {position.x - size.x/2.0f, position.y - size.y/2.0f, size.x, size.y};
        }
};
