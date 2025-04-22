#include "game.h"

void Game::run() {
	Input::get_singleton().add_listener(this);
	Input::get_singleton().start_input();

	_init();
	while(m_running) {
		using namespace std::chrono;
		auto pre_update = high_resolution_clock::now();

		_update((double)m_delta_time.count() / 1000.0);
		_draw();
		m_renderer.render_screen();

		auto post_update = high_resolution_clock::now();
		m_delta_time = duration_cast<milliseconds>(post_update - pre_update);
	}
}
