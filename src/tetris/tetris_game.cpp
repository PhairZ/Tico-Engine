#include "tetris_game.h"
#include "level.h"
#include <random>
#include <algorithm>

void Tetris::spawn_block(const Vector2& p_pos) {
	curr_block = std::move(next_block);

	std::random_device rd;
	std::default_random_engine gen(rd());
	
	std::uniform_int_distribution<> dist(1, Block::C_VARIATIONS);

	switch (dist(gen)) {
	case 1:
		next_block = std::make_unique<SquareTetroid>(p_pos, Renderer::BLACK, "[]");
		break;
	case 2:
		next_block = std::make_unique<TTetroid>(p_pos, Renderer::GREEN, "}{");
		break;
	case 3:
		next_block = std::make_unique<LTetroid>(p_pos, Renderer::RED, "[]");
		break;
	case 4:
		next_block = std::make_unique<RLTetroid>(p_pos, Renderer::BLUE, "}{");
		break;
	case 5:
		next_block = std::make_unique<STetroid>(p_pos, Renderer::MAGENTA, "[]");
		break;
	case 6:
		next_block = std::make_unique<RSTetroid>(p_pos, Renderer::YELLOW, "}{");
		break;
	case 7:
		next_block = std::make_unique<LineTetroid>(p_pos, Renderer::BLUE, "[]");
		break;
	default:
		break;
	}

	if (curr_block == nullptr) {
		return spawn_block();
	}
	curr_block->draw();
}

void Tetris::_init() {
	spawn_block();

	for (int y = 0; y <= Level::LVL_SIZE_Y; y++) {
		for (int x = 0; x <= Level::LVL_SIZE_X; x++) {
			if (x == 0 || y == Level::LVL_SIZE_Y || x == Level::LVL_SIZE_X) {
				Level::get_singleton().set_cell({ x, y }, { Renderer::CYAN } );
			} else {
				Level::get_singleton().set_cell({ x, y }, { Renderer::EMPTY_COLOR });
			}
		}
	}
}

void Tetris::_input_event(InputEvent* const p_event) {
	if (auto iek = dynamic_cast<InputEventKey*>(p_event)) {
		switch (iek->key) {
		case 'w':
			while (curr_block->move({ 0, 1 }) == 0) {}
			break;
		case 'a':
			curr_block->move({ -1, 0 });
			break;
		case 'd':
			curr_block->move({ 1, 0 });
			break;
		case 's':
			curr_block->move({ 0, 1 });
			break;
		case 'q':
			curr_block->rotate(true);
			break;
		case 'e':
			curr_block->rotate(false);
			break;
		case 27:
			m_running = false;
			break;
		default:
			break;
		}
	}
}

void Tetris::_update(double p_delta) {
	static double ticks = 0;
	ticks += 20.0 * p_delta;
	if (ticks > 10.0) {
		ticks = 0;
		if (curr_block->move({ 0, 1 }) == -1) {
			if (Level::get_singleton().get_cell(C_SPAWN_POS).color != Renderer::EMPTY_COLOR) {
				m_running = false;
				return;
			}
			switch (Level::get_singleton().clear_line(curr_block->get_position().y)) {
				case 1:
					m_singles++;
					break;
				case 2:
					m_doubles++;
					break;
				case 3:
					m_triples++;
					break;
				case 4:
					m_quadruples++;
			}
			spawn_block();
		}
	}
}

void Tetris::_draw() {
	// Level
	{
		for (int y = 0; y <= Level::LVL_SIZE_Y; y++) {
			for (int x = 0; x <= Level::LVL_SIZE_X; x++) {
				auto cell = Level::get_singleton().get_cell({ x, y });
				m_renderer.draw_pixel({ x, y }, Renderer::EMPTY_COLOR, cell.color, cell.texture);
			}
		}
	}

	// UI
	{
		const int UI_START = (Level::LVL_SIZE_X + 1) * 2;
		const int UI_WIDTH = 7;
		for (int y = 0; y <= Level::LVL_SIZE_Y; y++) {
			for (int x = UI_START / 2; x < UI_START / 2 + UI_WIDTH; x++) {
				m_renderer.draw_pixel({ x, y }, Renderer::EMPTY_COLOR, Renderer::CYAN);
			}
		}

		using RC = Renderer::Color;
		m_renderer.print(" TERTIS ", { UI_START + 1, 1 }, RC::BLACK, RC::WHITE);
		m_renderer.print(" Terminal  ", { UI_START, 3 }, RC::BLACK, RC::WHITE);
		m_renderer.print("    Tetris ", { UI_START, 4 }, RC::BLACK, RC::WHITE);
		m_renderer.print(" by", { UI_START, 5 }, RC::BLACK, RC::WHITE);
		m_renderer.print(" PhairZ ", {UI_START + 3, 5 }, RC::BLUE, RC::WHITE);

		char buf[8];
		snprintf(buf, 8, "%7d", m_singles + m_doubles * 2 + m_triples * 3  + m_quadruples * 4);
		m_renderer.print("Lines:", { UI_START, 8 }, RC::WHITE, RC::CYAN);
		m_renderer.print(buf, { UI_START + 6, 8 }, RC::BLACK, RC::WHITE);

		snprintf(buf, 8, "%7d", m_singles * 100 + m_doubles * 300 + m_triples * 500 + m_quadruples * 700);
		m_renderer.print("Score:", { UI_START, 10 }, RC::WHITE, RC::CYAN);
		m_renderer.print(buf, { UI_START + 6, 10 }, RC::BLACK, RC::WHITE);

		// Next Block.
		for (int y = 12; y < 16; y++) {
			for (int x = UI_START / 2 + 1; x < UI_START / 2 + 5; x++) {
				auto shape = next_block->get_shape();
				auto tile = std::find(
					shape.begin(), shape.begin() + 4,
					Vector2(x - 15, y - 13)
				);
				if (tile == shape.begin() + 4) {
					m_renderer.draw_pixel({ x, y }, RC::DEFAULT, RC::WHITE);
				} else {
					m_renderer.draw_pixel(
						{ x, y }, Renderer::EMPTY_COLOR,
						next_block->color, next_block->texture
					);
				}
			}
		}
	}
}