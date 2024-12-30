#include "../base/Sprite.cpp"
#include "../base/GameObject.cpp"
#include <string>


#pragma once
bool loaded;
Font font;

class Card: public GameObject, public std::enable_shared_from_this<Card> {
    private:
        bool last_isChosen = false;

        void resInit(){
            if (loaded){
                return;
            }
            loaded = true;

            // font = LoadFont("asset/NotoSans.ttf");

            char text[]="bel你好♠♥ ♦♣無牌型偶星三賢者雙偶星滿座序列同色四騎士同色順序";
            // 将字符串中的字符逐一转换成Unicode码点，得到码点表
            int codepointsCount;
            int *codepoints=LoadCodepoints(text,&codepointsCount);
            // 读取仅码点表中各字符的字体
            int fileSize;
            const unsigned char *fontData = LoadFileData("asset/NoToTc.ttf",&fileSize);
            font = LoadFontFromMemory(".ttf", fontData, fileSize, 32, codepoints, codepointsCount);
            // 释放码点表
            UnloadCodepoints(codepoints);
        }
        std::string suitMap(int s){
            switch (s){
                case SPADE:
                    return "♠";
                case HEART:
                    return "♥";
                case DIAMOND:
                    return "♦";
                case CLUB:
                    return "♣";
            }
            return "";
        }
        
    protected:
        int number;
        int suit;
        bool isFaceUp = false; // 是否正面朝上
        bool isHover = false; // 是否被悬停
        bool isChosen = false; // 是否被选中
        
        int state;

        Vector2 target_position;
        const Vector2 cardSize = {90.0f, 128.0f};
        std::shared_ptr<Sprite> backgroundSprite;

        bool isMouseInside(){ // 鼠标是否在精灵上
            Vector2 LT = position - cardSize/2.0f;
            Vector2 RD = position + cardSize/2.0f;
            Rectangle zone = {LT.x, LT.y, cardSize.x, cardSize.y};
            return CheckCollisionPointRec(GetMousePosition(), zone);
        }
        void moveCheck();

        void draw() override{
            GameObject::draw();
            
            DrawTextEx(GetFontDefault(), TextFormat("%d", number), position-cardSize/2.3f, 20.0f, 2.0f, WHITE); // 绘制数字
            DrawTextEx(font, suitMap(suit).c_str(), position-cardSize/2.9f, 20.0f, 2.0f, WHITE); // 绘制花色  
            backgroundSprite->setPosition(position); // 设置精灵的位置
            backgroundSprite->setTint(isHover && (state == HAND_ZONE) ? GRAY : WHITE); // 设置選中精灵的颜色
            if(isFaceUp){
                
            }
        }
        void update(float delta) override{ // 更新精灵
            
            GameObject::update(delta);
            if (state == HAND_ZONE){
                if (isChosen){ // 如果被选中，设置目标位置为鼠标位置
                    position = GetMousePosition();
                }else if (last_isChosen){
                    moveCheck();
                }
                last_isChosen = isChosen;
                isHover = false; // 重置选中状态
                isChosen = false; // 重置选中状态
            }
            if (Vector2Distance(position, target_position) > 1.0f){
                position += (target_position - position) * 4.5f * delta;
            }else{
                position = target_position;
            }
        }
        bool input () override{ // 输入
            bool ret = GameObject::input();
            if (ret){
                return true; // 如果有子对象被选中，返回true
            }
            if (state == HAND_ZONE){
                isHover = this->isMouseInside();
                isChosen = isHover && IsMouseButtonDown(MOUSE_LEFT_BUTTON);
                return isHover;
            }
            return false;
        }
    
    public: // 公有方法
        enum Suit{
            SPADE,
            HEART,
            DIAMOND,
            CLUB
        };
        enum CState{
            ENEMY_ZONE,
            PUBLIC_ZONE,
            PLAYER_ZONE,
            HAND_ZONE
        };
        Card(const char* path, int number, int suit){
            resInit();
            backgroundSprite = std::make_shared<Sprite>(path);
            addChild(backgroundSprite);
            this->number = number;
            this->suit = suit;
        }
        int getNumber(){
            return number;
        }
        int getSuit(){
            return suit;
        }
        void setTargetPosition(Vector2 target_position){ // 设置目标位置
            this->target_position = target_position;
        }
        void setState(CState state){ // 设置状态
            this->state = state;
            switch (state){
                case ENEMY_ZONE:
                    break;
                case PUBLIC_ZONE:
                    break;
                case PLAYER_ZONE:
                    break;
                case HAND_ZONE:
                    break;
            }
        }
        Rectangle getZone(){
            Vector2 LT = position - cardSize/2.0f;
            return {LT.x, LT.y, cardSize.x, cardSize.y};
        }
        std::string to_string(){
            return std::to_string(number) + suitMap(suit);
        }

};