#include "music_maker.h"
#include <cstring>
#include <cmath>
#include <fstream>

void MusicMaker::_init() {
	// try to load
	std::ifstream in("tetris.bin");

	if (in.is_open()) {
		std::cout << "opened" << std::endl;
		in.read(reinterpret_cast<char*>(m_notes.data()), m_notes.size() * sizeof(m_notes[0]));
		in.close();
	}
}

void MusicMaker::_input_event(InputEvent* const p_event) {
	if (auto iek = dynamic_cast<InputEventKey*>(p_event)) {
		if (iek->key >= '0' && iek->key <= '9' && m_notes[m_current_note].value != 0) {
			m_notes[m_current_note] = (m_notes[m_current_note].value - 1) % 12 + (iek->key - '0') * 12 + 1;
			update_audio();
		}
		switch (iek->key) {
		case 'w':
			m_current_note = (m_current_note + 199) % 200;
			break;
		case 's':
			m_current_note = (m_current_note + 1) % 200;
			break;
		case 'a':
			m_notes[m_current_note]--;
			update_audio();
			break;
		case 'd':
			m_notes[m_current_note]++;
			update_audio();
			break;
		case 'q':
			m_notes[m_current_note] = 0;
			update_audio();
			break;
		case 'l':
			save();
			break;
		case ' ':
			m_playing = !m_playing;
			if (m_playing) {
				m_audio.play();
			}
			else {
				m_audio.stop();
			}
			break;
		case 27:
			m_running = false;
			break;
		default:
			break;
		}
	}
}

// void MusicMaker::_init() {
	// m_audio.add_wave(Audio::SQR, 659, 250); // E5
	// m_audio.add_wave(Audio::SIN, 0, 100); // SILENCE
	// m_audio.add_wave(Audio::SQR, 494, 125); // B4
	// // m_audio.add_wave(Audio::SIN, 0, 100); // SILENCE
	// m_audio.add_wave(Audio::SQR, 523, 125); // C5
	// // m_audio.add_wave(Audio::SIN, 0, 100); // SILENCE
	// m_audio.add_wave(Audio::SQR, 587, 250); // D5
	// m_audio.add_wave(Audio::SIN, 0, 100); // SILENCE
	// m_audio.add_wave(Audio::SQR, 523, 125); // C5
	// // m_audio.add_wave(Audio::SIN, 0, 100); // SILENCE
	// m_audio.add_wave(Audio::SQR, 494, 125); // B4
	// // m_audio.add_wave(Audio::SIN, 0, 100); // SILENCE
 
	// m_audio.add_wave(Audio::SQR, 440, 250); // A4
	// m_audio.add_wave(Audio::SIN, 0, 100); // SILENCE
	// m_audio.add_wave(Audio::SQR, 440, 125); // A4
	// // m_audio.add_wave(Audio::SIN, 0, 100); // SILENCE
	// m_audio.add_wave(Audio::SQR, 523, 125); // C5
	// // m_audio.add_wave(Audio::SIN, 0, 100); // SILENCE
	// m_audio.add_wave(Audio::SQR, 659, 250); // E5
	// m_audio.add_wave(Audio::SIN, 0, 100); // SILENCE
	// m_audio.add_wave(Audio::SQR, 587, 125); // D5
	// // m_audio.add_wave(Audio::SIN, 0, 100); // SILENCE
	// m_audio.add_wave(Audio::SQR, 523, 125); // C5
	// // m_audio.add_wave(Audio::SIN, 0, 100); // SILENCE

	// m_audio.add_wave(Audio::SQR, 494, 250); // B4
	// m_audio.add_wave(Audio::SIN, 0, 100); // SILENCE
	// m_audio.add_wave(Audio::SQR, 494, 125); // B4
	// // m_audio.add_wave(Audio::SIN, 0, 100); // SILENCE
	// m_audio.add_wave(Audio::SQR, 523, 125); // C5
	// m_audio.add_wave(Audio::SIN, 0, 100); // SILENCE
	// m_audio.add_wave(Audio::SQR, 587, 250); // D5
	// m_audio.add_wave(Audio::SIN, 0, 100); // SILENCE
	// m_audio.add_wave(Audio::SQR, 659, 250); // E5
	// m_audio.add_wave(Audio::SIN, 0, 100); // SILENCE

	// m_audio.add_wave(Audio::SQR, 523, 250); // C5
	// m_audio.add_wave(Audio::SIN, 0, 100); // SILENCE
	// m_audio.add_wave(Audio::SQR, 440, 250); // A4
	// m_audio.add_wave(Audio::SIN, 0, 100); // SILENCE
	// m_audio.add_wave(Audio::SQR, 440, 250); // A4
	// m_audio.add_wave(Audio::SIN, 0, 100); // SILENCE
	// m_audio.play();
// }

void MusicMaker::_update(double p_delta) {
	// if (m_playing) {

	// }
}

void MusicMaker::_draw() {
	for (int i = 0; i < 20 * 10; i++) {
		char buf[5]; strcpy(buf, "   |");
		if (m_notes[i].value > 0) {
			snprintf(buf, 5, "%s%d|",
				m_notes[i].name.c_str(), (m_notes[i].value-1) / 12
			);
		}
		m_renderer.print(buf, { i/20 * 4, i % 20 },
			Renderer::EMPTY_COLOR,
			i == m_current_note ? (m_playing ? Renderer::RED : Renderer::BLUE) : Renderer::BLACK
		);
	}
}

void MusicMaker::update_audio() {
	m_audio.clear();
	for (int i = 0; i < 200; i++) {
		int freq = (m_notes[i].value > 0? round(440.0f * pow(2, (float)(m_notes[i].value - 57) / 12.0)) : 0);
		m_audio.add_wave(Audio::SQR, freq, 100);
	}
}

void MusicMaker::save() {
	std::ofstream out("tetris.bin");

	if (out.is_open()) {
		out.write(reinterpret_cast<char*>(&m_notes), sizeof(m_notes));
		out.close();
	} else {
		m_renderer.print("Failed to save");
	}
}
