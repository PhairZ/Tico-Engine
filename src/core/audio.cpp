#include "audio.h"
#include <cmath>
#include <thread>

int Audio::callback(
	const void* p_input, void* p_output,
	unsigned long p_frames_per_buffer,
	const PaStreamCallbackTimeInfo* p_time_info,
	PaStreamCallbackFlags p_status_flags,
	void* p_data
) {
	short*  out = (short*)p_output;
	(void) p_input; // Removes unused param warn.

	auto samples = *(std::vector<Sample>*)p_data;
	static int next_sample_idx = 0;

	static Sample* curr_sample = nullptr;
	static int curr_frame = 0;
	static float phase = 0;
	for (int i = 0; i < p_frames_per_buffer; i++, curr_frame++) {
		if (samples.begin() + next_sample_idx == samples.end()) {
			next_sample_idx = 0;
			curr_sample = nullptr;
			curr_frame = 0;
			delete (std::vector<Sample>*)p_data;
			return paComplete;
		}

		if (curr_frame >= samples[next_sample_idx].position_frames) {
			curr_sample = &samples[next_sample_idx];
			next_sample_idx++;
		}

		short output;
		switch (curr_sample->type) {
		case SIN:
			break;
		case SQR: {
			float phase_increment = ((float)curr_sample->freq / C_SAMPLE_RATE);
			if (phase >= 1) {
				phase -= 1;
			}
			output = 0x7fff * curr_sample->gain * (phase < 0.5f ? -1 : 1);

			phase += phase_increment;
			break;
		}
		default:
			break;
		}

		*out++ = output;
		*out++ = output;
	}

	return paContinue;
}

Audio::Audio() {
	PaError err = Pa_Initialize();
	if (err != paNoError) {
		const char* errr = Pa_GetErrorText(err);
		errr++;
	}
}

Audio::~Audio() {
	PaError err = Pa_Terminate();
	if (err != paNoError) {
		const char* errr = Pa_GetErrorText(err);
		errr++;
	}
}

void Audio::add_wave(SampleType p_type, int p_freq, int p_duration_ms, float p_gain) {
	Sample data{p_type, p_freq, m_total_frames, p_gain};
	m_total_frames += (p_duration_ms * C_SAMPLE_RATE) / 1000;
	m_sequence.push_back(data);
}

void Audio::clear() {
	m_sequence.clear();
	m_total_frames = 0;
}

void Audio::play() {
	std::lock_guard<std::mutex> lock(m_mutex);
	if (m_stream && Pa_IsStreamActive(m_stream)) {
		Pa_AbortStream(m_stream);
		Pa_CloseStream(m_stream);
		m_stream = nullptr;
	}

	auto* data = new std::vector<Sample>(m_sequence);
	// Dummy sample to find duration of last sample.
	data->push_back({SIN, 0, m_total_frames, 0.0});

	PaError err;
	err = Pa_OpenDefaultStream(
		&m_stream,
		0,
		2,
		paInt16,
		C_SAMPLE_RATE,
		1024,
		callback,
		data
	);
	if (err != paNoError)
		return;

	err = Pa_StartStream(m_stream);
	if (err != paNoError)
		return;
	
	std::thread([this] {
		m_mutex.lock();
		PaStream* local_stream = m_stream;
		m_mutex.unlock();

		while (Pa_IsStreamActive(local_stream) > 0) {
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		
		std::lock_guard<std::mutex> lock(m_mutex);
		if (local_stream == m_stream) {
			PaError err;
			err = Pa_StopStream(m_stream);
			if (err != paNoError)
				return;

			Pa_CloseStream(m_stream);
		}
	}).detach();
}

void Audio::stop() {
	std::lock_guard<std::mutex> lock(m_mutex);
	if (m_stream && Pa_IsStreamActive(m_stream)) {
		Pa_StopStream(m_stream);
		Pa_CloseStream(m_stream);
		m_stream = nullptr;
	}
}