#include <raylib.h>
#include <raymath.h>
#include <vector>
#include <memory>
#include "GameObject.cpp"
#include "Card.cpp"
#include "VectorUtils.cpp"

#pragma once
using namespace VectorUtils;
class HandDeckZone:public GameObject {
    private:
        Vector2 position = {420.0f, 548.0f}; // 手牌区域中心位置
        Vector2 size = {760.0f, 150.0f}; // 手牌区域大小
        std::vector<std::unique_ptr<Card>> cards; // 手牌
    public:
        HandDeckZone() = default;

        void draw() override{ // 绘制手牌
            GameObject::draw();
            Vector2 pos = position - size/2.0f; // 计算手牌区域左上角位置
            DrawRectangleV(pos, size, RED); // 绘制手牌区域

            int topCardIndex = -1; // 最上面的手牌索引
            for (int i = 0; i < cards.size(); i++){ // 遍历手牌
                if (topCardIndex==-1 && cards[i]->isMouseInside()){ // 如果鼠标在手牌上
                    topCardIndex = i;
                }else{
                    cards[i]->setTopCard(false);
                    cards[i]->draw();
                }
            }
            if (topCardIndex != -1){
                cards[topCardIndex]->setTopCard(true);
                cards[topCardIndex]->draw();
            }
            
            
        }
        void update(float delta) override{ // 更新手牌
            GameObject::update(delta);

            const float cardInterval = 60.0f;
            Vector2 cardSize = {90.0f, 128.0f};
            Vector2 cardPos = position - Vector2{cardInterval * (float)(cards.size()/2.0f), 0.0f}; // 计算第一张手牌的位置

            for (int i = 0; i < cards.size(); i++){
                cards[i]->setPosition(cardPos);
                cardPos.x += cardInterval; // 更新下一张手牌的位置
                cards[i]->update(delta);
            }
        }
        void addCard(std::unique_ptr<Card> card){ // 添加手牌
            cards.push_back(std::move(card));
        }
        void removeCard(Card* card){ // 移除手牌
            cards.erase(std::remove_if(cards.begin(), cards.end(), [card](const std::unique_ptr<Card>& p) { return p.get() == card; }), cards.end());
        }
};