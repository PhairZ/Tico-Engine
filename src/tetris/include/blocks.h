#pragma once

#include "objects.h"
#include "data.h"
#include "renderer.h"
#include <vector>

class Block : Object {
public:
    Block() :
        color(Renderer::DEFAULT), m_position(Vector2()) {}
    Block(const Vector2& p_pos, Renderer::Color p_color, const char* p_tex = "[]") :
        color(p_color), texture(p_tex), m_position(p_pos) {}

    const Vector2& get_position() const { return m_position; }

    void draw();
    int move(const Vector2& p_offset);
    int rotate(bool p_ccw = false);

    const Renderer::Color color;
    const char* texture;

    static constexpr int VARIATIONS = 7;

    virtual const std::vector<Vector2> get_shape() const {
        return { Vector2() };
    }
private:
    void clear();

    Vector2 m_position;
    int m_rot_offset = 0;
};

class SquareTetroid : public Block {
public:
    using Block::Block;

    const std::vector<Vector2> get_shape() const override {
        return {
            Vector2(0, 0),
            Vector2(0, 1),
            Vector2(1, 0),
            Vector2(1, 1)
        };
    }
};

class TTetroid : public Block {
public:
    using Block::Block;

    const std::vector<Vector2> get_shape() const override {
        return {
            Vector2(0, 0),
            Vector2(0, 1),
            Vector2(1, 0),
            Vector2(-1, 0),

            Vector2(0, 0),
            Vector2(-1, 0),
            Vector2(0, -1),
            Vector2(0, 1),

            Vector2(0, 0),
            Vector2(0, -1),
            Vector2(1, 0),
            Vector2(-1, 0),

            Vector2(0, 0),
            Vector2(1, 0),
            Vector2(0, -1),
            Vector2(0, 1)
        };
    }
};

class LTetroid : public Block {
public:
    using Block::Block;

    const std::vector<Vector2> get_shape() const override {
        return {
            Vector2(0, 0),
            Vector2(-1, 0),
            Vector2(1, 0),
            Vector2(1, 1),

            Vector2(0, 0),
            Vector2(0, -1),
            Vector2(0, 1),
            Vector2(-1, 1),

            Vector2(0, 0),
            Vector2(1, 0),
            Vector2(-1, 0),
            Vector2(-1, -1),

            Vector2(0, 0),
            Vector2(0, 1),
            Vector2(0, -1),
            Vector2(1, -1)
        };
    }
};

class RLTetroid : public Block {
public:
    using Block::Block;

    const std::vector<Vector2> get_shape() const override {
        return {
            Vector2(0, 0),
            Vector2(1, 0),
            Vector2(-1, 0),
            Vector2(-1, 1),

            Vector2(0, 0),
            Vector2(0, 1),
            Vector2(0, -1),
            Vector2(-1, -1),

            Vector2(0, 0),
            Vector2(-1, 0),
            Vector2(1, 0),
            Vector2(1, -1),

            Vector2(0, 0),
            Vector2(0, -1),
            Vector2(0, 1),
            Vector2(1, 1)
        };
    }
};

class STetroid : public Block {
public:
    using Block::Block;

    const std::vector<Vector2> get_shape() const override {
        return {
            Vector2(0, 0),
            Vector2(1, 0),
            Vector2(0, 1),
            Vector2(-1, 1),

            Vector2(0, 0),
            Vector2(0, 1),
            Vector2(-1, 0),
            Vector2(-1, -1)
        };
    }
};

class RSTetroid : public Block {
public:
    using Block::Block;

    const std::vector<Vector2> get_shape() const override {
        return {
            Vector2(0, 0),
            Vector2(-1, 0),
            Vector2(0, 1),
            Vector2(1, 1),

            Vector2(0, 0),
            Vector2(0, -1),
            Vector2(-1, 0),
            Vector2(-1, 1)
        };
    }
};

class LineTetroid : public Block {
public:
    using Block::Block;

    const std::vector<Vector2> get_shape() const override {
        return {
            Vector2(0, 0),
            Vector2(-1, 0),
            Vector2(1, 0),
            Vector2(2, 0),

            Vector2(0, 0),
            Vector2(0, -1),
            Vector2(0, 1),
            Vector2(0, 2)
        };
    }
};
