#pragma once

#include "game.h"
#include "data.h"
#include "blocks.h"
#include <vector>
#include <memory>

class Tetris : public Game {
private:
    void _init() override;
	void _input_event(InputEvent* p_event) override;
    void _update(double p_delta) override;
    void _draw() override;

    void spawn_block(const Vector2& p_pos = S_SPAWN_POS);

    static constexpr Vector2 S_SPAWN_POS{ 6, 2 };

    int m_singles = 0;
    int m_doubles = 0;
    int m_triples = 0;
    int m_quadruples = 0;

    std::unique_ptr<Block> curr_block = nullptr;
    std::unique_ptr<Block> next_block = nullptr;
};