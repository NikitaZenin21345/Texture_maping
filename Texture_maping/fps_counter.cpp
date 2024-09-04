#include "fps_counter.h"



void Fps_counter::update()
{
	auto now = std::chrono::high_resolution_clock::now();

	if (auto elapsed = now - m_last_update; elapsed > m_measure_interval) {
		size_t fps = m_frames * std::chrono::milliseconds(1000) / elapsed;
		m_last_update = now;
		m_frames = 0;

		if (fps != m_fps) {
			m_fps = fps;
			emit fps_changed(m_fps);
		}
	}
	else {
		++m_frames;
	}
}
