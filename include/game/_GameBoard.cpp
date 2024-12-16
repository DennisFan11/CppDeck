
#include "EnemyZone.cpp" // 敌方区域
#include "PublicZone.cpp" // 公共区域
#include "PlayerZone.cpp" // 玩家区域
#include "HandZone.cpp" // 手牌区域
#include "TrashCanZone.cpp" // 垃圾桶区域

#include "Card.cpp"

#define RAYGUI_IMPLEMENTATION

#include "../base/raygui.h"

#pragma once
class _GameBoard;
_GameBoard* gameBoard;
class _GameBoard:public GameObject, public std::enable_shared_from_this<_GameBoard>{
    private:
        void Gui(){
            if (GuiButton((Rectangle){ 980, 100, 120, 30 }, "add card")){
                printf("add card\n");
                std::shared_ptr<Card> a_card = std::make_shared<Card>("asset/Deck.png", 1, 1); //创建精灵对象
                a_card->setPosition({0.0f, 0.0f});
                a_card->setTargetPosition({0.0f, 0.0f});
                handZone->addChild(a_card); //将精灵对象添加到手牌区域对象中
            }
            if (GuiButton((Rectangle){ 980, 150, 120, 30 }, "remove card")){
                handZone->removeChild(0);
            }
        }
        void draw() override{
            GameObject::draw();
            Gui();
        }
        void update(float delta) override{
            GameObject::update(delta);
        }
        bool input() override{
            return GameObject::input();
        }
    protected:
        
    public:
        int test_key = 50;
        std::shared_ptr<TrashCanZone> trashCanZone;
        std::shared_ptr<EnemyZone> enemyZone;
        std::shared_ptr<PublicZone> publicZone;
        std::shared_ptr<PlayerZone> playerZone;
        std::shared_ptr<HandZone> handZone;
        
        // static std::shared_ptr<_GameBoard> gameBoard;
        _GameBoard(){
            gameBoard = this;
            trashCanZone = std::make_shared<TrashCanZone>();
            enemyZone = std::make_shared<EnemyZone>();
            publicZone = std::make_shared<PublicZone>();
            playerZone = std::make_shared<PlayerZone>();
            handZone = std::make_shared<HandZone>();
            
            addChild(trashCanZone);
            addChild(enemyZone);
            addChild(publicZone);
            addChild(playerZone);
            addChild(handZone);
            
        }
        Rectangle getPublicZone(){
            return publicZone->getZone();
        }
        Rectangle getTrashCanZone(){
            return trashCanZone->getZone();
        }
        
};

void Card::moveCheck(){
    // _GameBoard *gameBoard = (_GameBoard*)gameBoard;
    // printf("moveCheck, inside %d\n", CheckCollisionPointRec(GetMousePosition(), gameBoard->trashCanZone->getZone()));
    // printf("test_key %d\n", gameBoard->test_key);

    if (CheckCollisionRecs(getZone(), gameBoard->trashCanZone->getZone())){
        printf("-> trashCan zone\n");
        // gameBoard->trashCanZone->addChild(shared_from_this());
        gameBoard->handZone->removeChild(shared_from_this());
    }
    if (CheckCollisionRecs(getZone(), gameBoard->playerZone->getZone())){
        printf("-> playerZone\n");
        gameBoard->playerZone->addChild(shared_from_this());
        gameBoard->handZone->removeChild(shared_from_this());
    }
}