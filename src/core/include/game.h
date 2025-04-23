#pragma once

#include "input.h"
#include "renderer.h"
#include <chrono>

class Game : public InputEventListener {
public:
    virtual ~Game() = default;

    void run();
protected:
    virtual void _init() {}
	virtual void _input_event(InputEvent* p_event) override {}
    virtual void _update(double p_delta) {}
    virtual void _draw() {}

    bool m_running = true;
    Renderer m_renderer = Renderer({ 40, 20 });
    std::chrono::milliseconds m_delta_time{0};
};
