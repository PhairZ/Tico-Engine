#pragma once

#include "data.h"
#include <vector>

#define COLOR_BLACK "\033[40m"
#define COLOR_RED "\033[41m"
#define COLOR_GREEN "\033[42m"
#define COLOR_YELLOW "\033[43m"
#define COLOR_BLUE "\033[44m"
#define COLOR_MAGENTA "\033[45m"
#define COLOR_CYAN "\033[46m"
#define COLOR_WHITE "\033[47m"
#define COLOR_DEFAULT "\033[49m"

#define TEXT_COL_BLACK "\033[30m"
#define TEXT_COL_RED "\033[31m"
#define TEXT_COL_GREEN "\033[32m"
#define TEXT_COL_YELLOW "\033[33m"
#define TEXT_COL_BLUE "\033[34m"
#define TEXT_COL_MAGENTA "\033[35m"
#define TEXT_COL_CYAN "\033[36m"
#define TEXT_COL_WHITE "\033[37m"
#define TEXT_COL_DEFAULT "\033[39m"

class Renderer {
public:
	enum Color {
		BLACK,
		RED,
		GREEN,
		YELLOW,
		BLUE,
		MAGENTA,
		CYAN,
		WHITE,
		DEFAULT,
	};

	Renderer() = delete;
	explicit Renderer(const Vector2& p_display_size);
	~Renderer();

	static constexpr Color EMPTY_COLOR = WHITE;

	void clear_screen();
	void draw_pixel(const Vector2& p_coord, const Color p_fg_color, const Color p_bg_color, const char* p_tex = "  ");
	void draw_chixel(const Vector2& p_coord, const Color p_fg_color, const Color p_bg_color, const char* p_tex = " ");
	void render_screen();

	void print(const char* p_str, const Vector2& p_coord = Vector2(), const Color p_fg_color = DEFAULT, const Color p_bg_color = DEFAULT);

	const Vector2 buf_size;
private:

	struct Chixel {
		Color fg = DEFAULT;
		Color bg = DEFAULT;
		char c = 0;
	};
	friend std::ostream& operator<<(std::ostream&, const Renderer::Chixel&);
	std::vector<Chixel> m_display_buf;
};
