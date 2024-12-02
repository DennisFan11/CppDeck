#include <raylib.h>
#include <raymath.h>
#include <iostream>
#pragma once
namespace VectorUtils{ // 为了避免命名冲突，将 Vector2 相关的函数放在 VectorUtils 命名空间中
    // 加法
    Vector2 operator+(const Vector2& v1, const Vector2& v2) {
        return { v1.x + v2.x, v1.y + v2.y };
    }
    // 减法
    Vector2 operator-(const Vector2& v1, const Vector2& v2) {
        return { v1.x - v2.x, v1.y - v2.y };
    }
    // 缩放（乘以 float）
    Vector2 operator*(const Vector2& v, float scale) {
        return { v.x * scale, v.y * scale };
    }
    // 缩放（除以 float）
    Vector2 operator/(const Vector2& v, float scale) {
        if (scale == 0.0f) throw std::invalid_argument("Division by zero");
        return { v.x / scale, v.y / scale };
    }
    // 乘法交换律（float 在左侧）
    Vector2 operator*(float scale, const Vector2& v) {
        return { v.x * scale, v.y * scale };
    }
    // 输出到 std::ostream
    std::ostream& operator<<(std::ostream& os, const Vector2& v) {
        os << "(" << v.x << ", " << v.y << ")";
        return os;
    }
}