#pragma once

#include "game.h"
#include <stab.h>

class Note {
public:
	u_char value = 0;
	std::string name = "  ";

	Note& operator+(int p_val) {
		value = (value + p_val) % (C_OCTAVES * C_NOTES + 1);
		name = (value == 0 ? "  " : C_TABLE[(value-1) % C_NOTES]);
		return *this;
	}
	Note& operator-(int p_val) {
		value = (value - p_val + (C_OCTAVES * C_NOTES + 1)) % (C_OCTAVES * C_NOTES + 1);
		name = (value == 0 ? "  " : C_TABLE[(value-1) % C_NOTES]);
		return *this;
	}
	Note& operator++(int) {
		return *this = *this + 1;
	}
	Note& operator--(int) {
		return *this = *this - 1;
	}
	Note& operator=(int p_val) {
		value = (p_val > C_OCTAVES * C_NOTES ? 0 : p_val);
		name = (value == 0 ? "  " : C_TABLE[(value-1) % C_NOTES]);
		return *this;
	}
private:
	static constexpr int C_OCTAVES = 10;
	static constexpr int C_NOTES = 12;
	static constexpr const char* C_TABLE[] = {
		"C ", "C#", "D ", "D#", "E ", "F ", "F#", "G ", "G#", "A ", "A#", "B " 
	};
};

class MusicMaker : public Game {
	void _init() override;
	void _input_event(InputEvent* const p_event) override;
	void _update(double p_delta) override;
	void _draw() override;

	void update_audio();
	void save();

	bool m_playing = false;
	int m_current_note = 0;
	std::array<Note, 1024> m_notes{};
};