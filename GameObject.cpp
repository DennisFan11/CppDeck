#include <raylib.h>
#include <vector>
#include <algorithm>

class GameObject
{
private:
    std::vector<GameObject&> children; // 子对象
public:
    GameObject(/* args */); // 构造函数
    ~GameObject(); // 析构函数
    virtual void update(float delta); // 每帧更新
    virtual void draw(); // 每帧绘制
    
    void addChild(GameObject &child); // 添加子对象
    void removeChild(GameObject &child); // 移除子对象
    std::vector<GameObject&> getChildren(); // 获取子对象
};



GameObject::GameObject(/* args */)
{
    /* nothing */
}

GameObject::~GameObject() // 析构函数 释放所有子对象
{
    for(auto &child:children){
        delete &child;
    }
}

void GameObject::update(float delta){ // 更新所有子对象
    for(auto &child:children){
        child.update(delta);
    }
}

void GameObject::draw(){ // 绘制所有子对象
    for(auto &child:children){
        child.draw();
    }
}

void GameObject::addChild(GameObject &child){ // 添加子对象
    children.push_back(child);
}

void GameObject::removeChild(GameObject &child){ // 移除子对象
    children.erase(std::remove(children.begin(), children.end(), child), children.end());
}

std::vector<GameObject&> GameObject::getChildren(){ // 获取子对象
    return children;
}