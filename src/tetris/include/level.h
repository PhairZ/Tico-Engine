#pragma once

#include "data.h"
#include "renderer.h"
#include "utils/singleton.h"
#include <array>

class Level;

class Cell {
public:
	Cell() :
		color(Renderer::EMPTY_COLOR), texture("  ") {}
	Cell(Renderer::Color p_color) :
		color(p_color), texture("  ") {}
	Cell(Renderer::Color p_color, const char* p_texture) :
		color(p_color), texture(p_texture) {}

	Renderer::Color color;
	const char* texture;
};

class Level {
	SINGLETON(Level) = default;
public:
	void set_cell(const Vector2& p_pos, const Cell& p_data);
	const Cell& get_cell(const Vector2& p_pos) const { return m_grid[p_pos.x + LVL_SIZE_X * p_pos.y]; };
	int clear_line(int p_level);

	static constexpr int LVL_SIZE_X = 12;
	static constexpr int LVL_SIZE_Y = 19;
private:

	static Level* s_instance;
	
	std::array<Cell, (LVL_SIZE_Y + 1) * (LVL_SIZE_Y + 1)> m_grid;
};
