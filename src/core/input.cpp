#include "input.h"
#include <iostream>

#ifdef _WIN32
#include <conio.h>

Input::Input() = default;
#else
#include <unistd.h>

Input::Input() {
    if (tcgetattr(STDIN_FILENO, &m_term_attr) < 0) {
        perror("tcgetattr()");
    }
    termios new_attr = m_term_attr;
    new_attr.c_lflag &= ~ICANON;
    new_attr.c_lflag &= ~ECHO;
    new_attr.c_cc[VMIN] = 1;
    new_attr.c_cc[VTIME] = 0;
    if (tcsetattr(STDIN_FILENO, TCSANOW, &new_attr) < 0) {
        perror("tcsetattr ICANON");
    }
#endif // _WIN32
}

Input::~Input() {
    m_reading = false;
    if (m_input_thread.joinable()) {
        m_input_thread.join();
    }
#ifndef _WIN32

    tcsetattr(STDIN_FILENO, TCSANOW, &m_term_attr);
#endif // _WIN32
}

char Input::getch() {
#ifdef _WIN32
    return _getch();
#else
    char buf = 0;
    if (read(STDIN_FILENO, &buf, 1) < 0)
        perror("read()");
    return buf;
#endif
}

void Input::add_listener(InputEventListener* p_listener) {
	m_listeners.emplace_back(p_listener);
}

void Input::start_input() {
    m_reading = true;
    if (!m_input_thread.joinable()) {
	    m_input_thread = std::thread(&Input::handle_input, this);
    }
}

void Input::handle_input() {
	while (m_reading) {
		std::unique_ptr<InputEventKey> iek = std::make_unique<InputEventKey>();
		iek->key = tolower(getch());
		iek->pressed = true;

        if (!m_reading) break;
		for (auto* listener : m_listeners) {
			listener->_input_event(iek.get());
		}
	}
}