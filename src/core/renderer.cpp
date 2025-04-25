#include "renderer.h"
#include <iostream>
#include <algorithm>
#ifdef _WIN32
#include <io.h>
#define null_device "NUL"
#else
#include <unistd.h>
#define null_device "/dev/null"
#endif // _WIN32

Renderer::Renderer(const Vector2& p_display_size) :
	buf_size(p_display_size),
	m_display_buf(std::vector<Chixel>(p_display_size.x * p_display_size.y)) {

    FILE* nullFile = fopen(null_device, "w");
    if (nullFile) {
        fflush(stderr);
        dup2(fileno(nullFile), fileno(stderr));
        fclose(nullFile);
    }
	
    std::cout << "\033[?25l"; // Hide Cursor.
	std::cout << "\033[=7h"; // Disable line wrapping.
	std::cout << "\033[=1049h"; // Enable Alternate Buffer.
	std::iostream::sync_with_stdio(false);
	std::cin.tie(nullptr);
}

Renderer::~Renderer(){
	// Refer to the constructor for more details.
	std::cout << "\033[?25h";
	std::cout << "\033[=7h";
	std::cout << "\033[=1049l";
}

void Renderer::draw_pixel(const Vector2& p_pos, const Color p_fg_color, const Color p_bg_color, const char* p_tex) {
	Chixel* ch = m_display_buf.data() + ((p_pos.x * 2) + buf_size.x * p_pos.y);
	if (ch) {
		if (p_fg_color != DEFAULT) {
			ch->fg = p_fg_color;
		}
		if (p_bg_color != DEFAULT) {
			ch->bg = p_bg_color;
		}
		if (p_tex[0]) {
			ch->c = p_tex[0];
		}
	}
	if (++ch) {
		if (p_fg_color != DEFAULT) {
			ch->fg = p_fg_color;
		}
		if (p_bg_color != DEFAULT) {
			ch->bg = p_bg_color;
		}
		if (p_tex[0] && p_tex[1]) {
			ch->c = p_tex[1];
		}
	}
}

void Renderer::draw_chixel(const Vector2& p_pos, const Color p_fg_color, const Color p_bg_color, const char* p_tex) {
	Chixel* ch = m_display_buf.data() + (p_pos.x + buf_size.x * p_pos.y);
	if (ch) {
		if (p_fg_color != DEFAULT) {
			ch->fg = p_fg_color;
		}
		if (p_bg_color != DEFAULT) {
			ch->bg = p_bg_color;
		}
		if (p_tex[0]) {
			ch->c = p_tex[0];
		}
	}
}

void Renderer::print(const char* p_str, const Vector2& p_pos, const Color p_fg_color, const Color p_bg_color) {
	Chixel* ch = m_display_buf.data() + (p_pos.x + buf_size.x * p_pos.y);
	for (int i = 0; p_str[i]; i++) {
		if (i + p_pos.x > buf_size.x - 1) break;

		if (p_fg_color != DEFAULT) {
			ch[i].fg = p_fg_color;
		}
		if (p_bg_color != DEFAULT) {
			ch[i].bg = p_bg_color;
		}
		ch[i].c = p_str[i];
	}
}

void move_cursor(int x, int y) {
	char buf[16] = "\033[999;999";
	snprintf(buf, 16, "\033[%d;%d", y + 1, x + 1);
	std::cout << buf;
}

void Renderer::render_screen() {
	std::cout << "\033[H";
	for (int y = 0; y < buf_size.y; y++) {
		for (int x = 0; x < buf_size.x; x++) {
			std::cout << m_display_buf[x + buf_size.x * y];
		}
		std::cout << std::endl;
	}
	std::cout << TEXT_COL_DEFAULT << COLOR_DEFAULT;
	std::cout.flush();
}

std::ostream& operator<<(std::ostream& p_os, const Renderer::Chixel& p_ch) {
	static constexpr const char* bg_col_table[Renderer::DEFAULT + 1] = {
		COLOR_BLACK,
		COLOR_RED,
		COLOR_GREEN,
		COLOR_YELLOW,
		COLOR_BLUE,
		COLOR_MAGENTA,
		COLOR_CYAN,
		COLOR_WHITE,
		COLOR_DEFAULT
	};
	static constexpr const char* fg_col_table[Renderer::DEFAULT + 1] = {
		TEXT_COL_BLACK,
		TEXT_COL_RED,
		TEXT_COL_GREEN,
		TEXT_COL_YELLOW,
		TEXT_COL_BLUE,
		TEXT_COL_MAGENTA,
		TEXT_COL_CYAN,
		TEXT_COL_WHITE,
		TEXT_COL_DEFAULT
	};

	return p_os << fg_col_table[p_ch.fg] << bg_col_table[p_ch.bg] << p_ch.c;
}