#include "level.h"

Level* Level::s_instance = new Level();

void Level::set_cell(const Vector2& p_pos, const Cell& p_data) {
    m_grid[p_pos.x + LVL_SIZE_X * p_pos.y] = p_data;
}

int Level::clear_line(int p_lvl) {
    int first_cleared = 0;
    int cleared_lines = 0;
    for (int y = std::max(1, p_lvl - 1); y < std::min((int)LVL_SIZE_Y, p_lvl + 3); y++) {
        bool cleared = true;
        for (int x = 1; x < LVL_SIZE_X; x++) {
            if (get_cell({ x, y }).color == Renderer::EMPTY_COLOR) {
                cleared = false;
                break;
            }
        }
        if (cleared) {
            first_cleared = y;
            cleared_lines++;
            for (int x = 1; x < LVL_SIZE_X; x++) {
                set_cell({ x, y }, Cell());
            }
        } else if (cleared_lines > 0) {
            break;
        }
    }

    for (int y = first_cleared; y >= cleared_lines && cleared_lines; y--) {
        for (int x = 1; x < LVL_SIZE_X; x++) {
            set_cell({ x, y }, get_cell({ x, y - cleared_lines }));
        }
    }

    return cleared_lines > 0 ? cleared_lines + clear_line(p_lvl) : 0;
}
