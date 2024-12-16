#include <raylib.h>
#include <raymath.h>
#include <vector>
#include <algorithm>
#include <memory>

#pragma once

class GameObject{
    protected:
        std::vector<std::shared_ptr<GameObject>> children; // 子对象列表
        Vector2 position; // 精灵的位置
        virtual void draw(){
            for (auto& child : children){
                child->draw();
            }
        }
        virtual void update(float delta){
            for (auto& child : children){
                // check if the child is nullptr
                if (child){
                    child->update(delta);
                }
            }
        }
        virtual bool input(){
            for (auto& child : children){
                if (child->input()){
                    return true;
                }
            }
            return false;
        }
    public:
        GameObject() = default;
        virtual ~GameObject() = default;
        
        void r_draw(){ // 递归绘制
            draw();
        }
        void r_update(float delta){ // 递归更新
            update(delta);
        }
        bool r_input(){ // 递归输入
            return input();
        }
        
        virtual void addChild(std::shared_ptr<GameObject> child){
            children.push_back(child);
        }
        void removeChild( std::shared_ptr<GameObject> child ){
            children.erase(std::remove(children.begin(), children.end(), child), children.end());
        }
        void removeChild( int index ){
            if (index < 0 || index >= children.size()){
                return;
            }
            children.erase(children.begin() + index);
        }
        void clearChildren(){

            children.clear();
        }
        void setPosition(Vector2 position){
            this->position = position;
        }
        Vector2 getPosition(){
            return position;
        }
        
        
};
