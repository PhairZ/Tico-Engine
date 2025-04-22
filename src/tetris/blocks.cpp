#include "blocks.h"
#include "level.h"
#include <algorithm>

void Block::draw() {
    Level& lvl = Level::get_singleton();
    auto shape = get_shape();
    for (size_t i = 0; i < 4; i++) {
        auto& tile =  shape[i + m_rot_offset];
        lvl.set_cell(m_position + tile, { color, texture });
    }
}

void Block::clear() {
    Level& lvl = Level::get_singleton();
    auto shape = get_shape();
    for (size_t i = 0; i < 4; i++) {
        auto& tile =  shape[i + m_rot_offset];
        lvl.set_cell(m_position + tile, Renderer::EMPTY_COLOR);
    }
}

int Block::move(const Vector2& p_offset) {
    Level& lvl = Level::get_singleton();
    auto shape = get_shape();
    for (size_t i = 0; i < 4; i++) {
        auto& tile = shape[i + m_rot_offset];
        bool is_self = std::any_of(
            shape.begin() + m_rot_offset, shape.begin() + 4 + m_rot_offset,
            [p_offset, tile](const Vector2& vec) { return vec == (tile + p_offset); }
        );

        if (!is_self) {
            bool collided = lvl.get_cell(m_position + p_offset + tile).color != Renderer::EMPTY_COLOR;
            if (collided)
                return -1;
        }
    }

    clear();
    m_position += p_offset;
    draw();
    return 0;
}

int Block::rotate(bool p_ccw) {
    Level& lvl = Level::get_singleton();
    auto shape = get_shape();
    const int new_rot = (m_rot_offset + (p_ccw ? -4 : 4)) % shape.size();
    for (size_t i = 0; i < 4; i++) {
        auto& new_shape = shape[i + new_rot];
        bool is_self = std::any_of(
            shape.begin() + m_rot_offset, shape.begin() + 4 + m_rot_offset,
            [new_shape](const Vector2& vec) { return vec == new_shape; }
        );

        if (!is_self) {
            bool collided = lvl.get_cell(m_position + new_shape).color != Renderer::EMPTY_COLOR;
            if (collided)
                return -1;;
        }
    }

    clear();
    m_rot_offset = new_rot;
    draw();
    return m_rot_offset;
}