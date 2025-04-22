#include "data.h"

Vector2 Vector2::operator+(const Vector2& p_other) const {
    return {x + p_other.x, y + p_other.y};
}

Vector2& Vector2::operator+=(const Vector2& other) {
    *this = *this + other;
    return *this;
}

bool Vector2::operator==(const Vector2& other) const {
    return (x == other.x && y == other.y);
}

std::ostream& operator<<(const std::ostream& p_os, const Vector2& p_vec2) {
    return p_os << '(' << p_vec2.x << ", " << p_vec2.y << ')';
}