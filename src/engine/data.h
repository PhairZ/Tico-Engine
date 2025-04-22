#pragma once

#include <iostream>

struct Vector2 {
public:
    int x, y;

    constexpr Vector2() :
        x(0), y(0) {}

    constexpr Vector2(int n) :
        x(n), y(n) {}

    constexpr Vector2(int x, int y) :
        x(x), y(y) {}

    constexpr Vector2(const Vector2& other) :
        x(other.x), y(other.y) {}

    Vector2 operator+(const Vector2& other) const;

    Vector2& operator+=(const Vector2& other);

    bool operator==(const Vector2& other) const;
};