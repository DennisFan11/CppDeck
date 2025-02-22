#include "../base/Sprite.cpp"
#include "../base/GameObject.cpp"
#include <string>


#pragma once
bool loaded;
Font font;
std::string getRule(){
    std::string prompt = "以下是遊戲規則：\n\n";
    prompt += "1. 遊戲目標：\n";
    prompt += "   - 從手牌中選擇最多 2 張牌，放入出牌區域，最大化得分。\n";
    prompt += "   - 遊戲會根據計分邏輯進行得分計算。\n\n";
    prompt += "2. 計分規則：\n";
    prompt += "   - 同色順序: 120 分 + 卡片總點數（同花色且連續的牌）。\n";
    prompt += "   - 四騎士: 80 分 + 卡片總點數（四張相同點數的牌）。\n";
    prompt += "   - 同色: 70 分 + 卡片總點數（至少五張同花色的牌）。\n";
    prompt += "   - 序列: 60 分 + 卡片總點數（五張連續的牌，不限花色）。\n";
    prompt += "   - 滿座: 55 分 + 卡片總點數（三張相同 + 兩張相同點數的牌）。\n";
    prompt += "   - 雙偶星: 30 分 + 卡片總點數(兩組兩張相同點數的牌，且兩組點數不相同)。\n";
    prompt += "   - 三賢者:30 分 + 卡片總點數(三張相同的牌，且其他牌沒辦法組出滿座)。\n";
    prompt += "   - 偶星:15 分 + 卡片總點數(兩張相同點數的牌)。\n";
    prompt += "   - 若無牌型，僅計算所有卡片的總點數。\n\n";
    prompt += "3. 策略建議：\n";
    prompt += "   - 優先構成高分牌型（同色順序 > 四騎士 > 同色 > 序列 > 滿座 > 雙偶星 = 三賢者 > 偶星）。\n";
    prompt += "   - 若無法構成高分牌型，選擇點數最大的卡片組合。\n";
    return prompt;
}

class Card: public GameObject, public std::enable_shared_from_this<Card> {
    private:
        bool last_isChosen = false;

        void resInit(){
            if (loaded){
                return;
            }
            loaded = true;

            // font = LoadFont("asset/NotoSans.ttf");

            std::string text = "bel你好♠♥ ♦♣無牌型偶星三賢者雙偶星滿座序列同色四騎士同色順序" + getRule();
            // 将字符串中的字符逐一转换成Unicode码点，得到码点表
            int codepointsCount;
            int *codepoints=LoadCodepoints(text.c_str(), &codepointsCount);
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