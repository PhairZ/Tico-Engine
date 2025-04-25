#pragma once

#include "portaudio.h"
#include <vector>
#include <mutex>

class Audio {
public:
	enum SampleType {
		SIN,
		SQR,
		SAW,
	};

	Audio();
	~Audio();

	void add_wave(SampleType p_type, int p_freq, int p_duration_ms, float p_gain = 1.0);
	void clear();
	void play();
	void stop();
private:
	static int callback(
		const void* p_input, void* p_output,
		unsigned long p_frames_per_buffer,
		const PaStreamCallbackTimeInfo* p_time_info,
		PaStreamCallbackFlags p_status_flags,
		void* p_data
	);
	static constexpr unsigned int C_SAMPLE_RATE = 16000;

	struct Sample {
		Audio::SampleType type;
		int freq, position_frames;
		float gain;
	};

	int m_total_frames = 0;
	PaStream* m_stream = nullptr;
	std::mutex m_mutex;
	std::vector<Sample> m_sequence;
};