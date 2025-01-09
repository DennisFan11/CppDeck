
#include "EnemyZone.cpp" // 敌方区域
#include "PublicZone.cpp" // 公共区域
#include "PlayerZone.cpp" // 玩家区域
#include "HandZone.cpp" // 手牌区域
#include "TrashCanZone.cpp" // 垃圾桶区域

#include "PointCounter.cpp"

#include "OpenAi_Enemy.cpp"

#include "Card.cpp"

#define RAYGUI_IMPLEMENTATION

#include "../base/raygui.h"

#pragma once



enum STATUS{ GAME_START, RULE_EXPLANE, ROUND, ROUND_END, GAME_END };

int _status = GAME_START; // 
int round_time = 0;
bool playerMoveEnd = false;
int playMovedCardCount = 0;
bool aiMoveEnd = false;

class _GameBoard;
_GameBoard* gameBoard;

class _GameBoard:public GameObject, public std::enable_shared_from_this<_GameBoard>{
    private:
        std::string message;
        void print_text(){
            DrawText(message.c_str(), 850.0f, 150.0f, 20, BLACK);
        }
        float _time = 0.0f;
        bool showRule = false;
        void Gui(){
            if (_status == ROUND){
                if (GuiButton((Rectangle){ 980, 100, 120, 30 }, "end turn")){
                    playerMoveEnd = true;
                }
            }
            if (GuiButton((Rectangle){ 980, 150, 120, 30 }, "show rule")){
                showRule = !showRule;
            }
            // if (GuiButton((Rectangle){ 980, 100, 120, 30 }, "add card")){
            //     printf("add card\n");
            //     int number = rand()%13 + 1;
            //     int suit = rand()%4;
            //     std::shared_ptr<Card> a_card = std::make_shared<Card>("asset/Deck.png", number, suit); //创建精灵对象
            //     a_card->setPosition({0.0f, 0.0f});
            //     a_card->setTargetPosition({0.0f, 0.0f});
            //     handZone->addChild(a_card); //将精灵对象添加到手牌区域对象中
            // }
            // if (GuiButton((Rectangle){ 980, 150, 120, 30 }, "remove card")){
            //     handZone->removeChild(0);
            // }
        }
        void draw() override{
            if (_status == GAME_START){
                DrawText("Game Start", 200, 200, 60, BLACK);
            }
            // else if (_status == RULE_EXPLANE){
            //     DrawText("RULE EXPLANE", 200, 200, 20, BLACK);
            // }
            // else if (_status == GAME_END){
            //     DrawTexture(game_over_texture, 0, 0, WHITE);
            else{
                GameObject::draw();
                if (showRule){
                    DrawRectangle(0, 0, 1152, 648, {0, 0, 0, 200});
                    DrawTextEx(font, getRule().c_str(), {100.0f, 100.0f}, 20.0f, 2.0f, WHITE);
                }
                Gui();
                print_text();
            }
            
        }
        void update(float delta) override{
            GameObject::update(delta);
            _countPoint();
            status_do();
            _time += delta;
        }
        
        typedef struct _card{
            int number;
            int suit;
        } _card;

        std::vector<_card> totCards;
        void gen_cards(){
            for (int i = 1; i <= 13; i++){
                for (int j = 0; j < 4; j++){
                    _card a_card;
                    a_card.number = i;
                    a_card.suit = j;
                    totCards.push_back(a_card);
                }
            }
        }
        
        std::shared_ptr<Card> getCard(){
            int index = rand()%totCards.size();
            _card a_card = totCards[index];
            totCards.erase(totCards.begin() + index);
            std::shared_ptr<Card> card = std::make_shared<Card>("asset/card_bg.png", a_card.number, a_card.suit);
            card->setPosition({0.0f, 0.0f});
            card->setTargetPosition({0.0f, 0.0f});
            return card;
        }

        void aiMove(){ // TODO:
            ai.cardZone = playerZone->to_string();
            ai.publicZone = publicZone->to_string();


            std::vector<int> move = ai.aiMove(); // call ai
            std::vector<std::shared_ptr<Card>> temp;
            for (int i = 0; i < move.size(); i++){
                if (move[i] >= 0 && move[i] < ai.handcards.size()) {
                    std::shared_ptr<Card> card = ai.handcards[move[i]];
                    temp.push_back(card);
                    enemyZone->addChild(card);
                }
            }
            for (auto& card : temp){
                ai.handcards.erase(std::remove(ai.handcards.begin(), ai.handcards.end(), card), ai.handcards.end());
            }
            aiMoveEnd = true;
        }
        
        
        void status_do(){
            switch (_status){
                case GAME_START:
                    message = "Game Start";

                    if(_time >= 3.0){
                        _status = RULE_EXPLANE;
                    }
                    break;
                case RULE_EXPLANE:
                    message = "Rule Explane";
                    if (true){ // TODO:
                        _status = ROUND;
                        playerMoveEnd = false;
                        playMovedCardCount = 0;
                        gen_cards();
                        _round_end_get_cards();
                    }
                    break;
                case ROUND:
                    message = "Round " + std::to_string(round_time+1) + "\n Player Turn" + "\n Left \"" + std::to_string(2 - playMovedCardCount) + "\" cards can move";
                    if (playerMoveEnd){ // TODO:
                        round_time++;
                        _status = ROUND_END;
                        aiMoveEnd = false;
                        
                    }
                    break;
                case ROUND_END: // ai move
                    // aiMoveEnd = true;
                    // ai move end
                    if (round_time >=3){
                        _status = GAME_END;
                        return;
                    }
                    aiMove();
                    message = "Round " + std::to_string(round_time+1) + "\n Enemy Turn";
                    if (aiMoveEnd){
                        _round_end_get_cards();
                        _status = ROUND;
                        playerMoveEnd = false;
                        playMovedCardCount = 0;
                    }
                    break;

                case GAME_END: // TODO:
                    message = "Game End";
                    // ExitGame();
                    break;
            } // end switch
            
        }// end status_do
        void _round_end_get_cards(){ // get 5 cards to player and enemy
            for(int i = 0; i < 5; i++){
                handZone->addChild(getCard());
                ai.handcards.push_back(getCard());
            }
            for(int i = 0; i < 2; i++){
                publicZone->addChild(getCard());
            }
        }



        bool input() override{
            return GameObject::input();
        }
        void _countPoint(){
            std::vector<int> publicNumber;
            std::vector<int> publicSuit;
            std::vector<int> playerNumber;
            std::vector<int> playerSuit;
            std::vector<int> enemyNumber;
            std::vector<int> enemySuit;
            for (auto& child : publicZone->getChildren()){
                publicNumber.push_back(std::dynamic_pointer_cast<Card>(child)->getNumber());
                publicSuit.push_back(std::dynamic_pointer_cast<Card>(child)->getSuit());
            }
            for (auto& child : playerZone->getChildren()){
                playerNumber.push_back(std::dynamic_pointer_cast<Card>(child)->getNumber());
                playerSuit.push_back(std::dynamic_pointer_cast<Card>(child)->getSuit());
            }
            for (auto& child : enemyZone->getChildren()){
                enemyNumber.push_back(std::dynamic_pointer_cast<Card>(child)->getNumber());
                enemySuit.push_back(std::dynamic_pointer_cast<Card>(child)->getSuit());
            }
            // std::cout << "publicCards: ";
            // for (int i = 0; i < publicNumber.size(); i++){
            //     std::cout << publicNumber[i] << " ";
            // }
            // std::cout << std::endl;
            // std::cout << "playerCards: ";
            // for (int i = 0; i < playerNumber.size(); i++){
            //     std::cout << playerNumber[i] << " ";
            // }
            // std::cout << std::endl;
            // std::cout << "enemyCards: ";
            // for (int i = 0; i < enemyNumber.size(); i++){
            //     std::cout << enemyNumber[i] << " ";
            // }
            // std::cout << std::endl;
            playerZone->point = countPoint(publicNumber, publicSuit, playerNumber, playerSuit);
            enemyZone->point = countPoint(publicNumber, publicSuit, enemyNumber, enemySuit);
            playerZone->count_s = countPoint_s(publicNumber, publicSuit, playerNumber, playerSuit);
            enemyZone->count_s = countPoint_s(publicNumber, publicSuit, enemyNumber, enemySuit);
            // std::cout << "player point: " << playerZone->point << std::endl;
            // std::cout << "enemy point: " << enemyZone->point << std::endl;
            // std::cout << "player count_s: " << playerZone->count_s << std::endl;
            // std::cout << "enemy count_s: " << enemyZone->count_s << std::endl;
        }
    protected:
        OpenAi_Enemy ai;
        Texture2D game_over_texture;
        Texture2D game_start_texture;
        // Texture2D rule_explane_texture;
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
            ai = OpenAi_Enemy();
            
            addChild(trashCanZone);
            addChild(enemyZone);
            addChild(publicZone);
            addChild(playerZone);
            addChild(handZone);
            // game_start_texture = LoadTexture("asset/start.png");
            game_over_texture = LoadTexture("asset/game_over.png");
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
    if ( _status != ROUND || playerMoveEnd){
        return;
    }
    if (playMovedCardCount >= 2){
        return;
    }
    if (CheckCollisionRecs(getZone(), gameBoard->trashCanZone->getZone())){
        // printf("-> trashCan zone\n");
        // gameBoard->trashCanZone->addChild(shared_from_this());
        gameBoard->handZone->removeChild(shared_from_this());
    }
    if (CheckCollisionRecs(getZone(), gameBoard->playerZone->getZone())){
        playMovedCardCount++;
        // printf("-> playerZone\n");
        gameBoard->playerZone->addChild(shared_from_this());
        gameBoard->handZone->removeChild(shared_from_this());
    }
}

















//