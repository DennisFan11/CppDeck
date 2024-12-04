#include <raylib.h>
#include <raymath.h>
#include <vector>
#include <algorithm>
#include <memory>

#pragma once

class GameObject{
    private:
        std::vector<std::unique_ptr<GameObject>> children; // 子对象列表
    protected:
        Vector2 position; // 精灵的位置
    public:
        GameObject() = default;
        virtual ~GameObject() = default;

        virtual void draw(){
            for (auto& child : children){
                child->draw();
            }
        }
        virtual void update(float delta){
            for (auto& child : children){
                child->update(delta);
            }
        }
        void addChild(std::unique_ptr<GameObject> child){
            children.push_back(std::move(child));
        }
        void removeChild(GameObject* child){
            children.erase(std::remove_if(children.begin(), children.end(), [child](const std::unique_ptr<GameObject>& p) { return p.get() == child; }), children.end());
        }
        void clearChildren(){
            children.clear();
        }
        std::vector<std::unique_ptr<GameObject>>& getChildren(){
            return children;
        }
        void setPosition(Vector2 position){
            this->position = position;
        }
        Vector2 getPosition(){
            return position;
        }
        
};
