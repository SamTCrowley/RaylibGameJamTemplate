#include "Anim.h"

Anim::Anim() { //  default constructor...
	frames = std::vector<int>({ 0 });

	frames_per_second = 0.0f;
	seconds_per_frame = 0.0f;
	counter = 0.0f;
	current_index = 0;
	current_frame = frames[current_index];
}

Anim::Anim(std::initializer_list<int> list, float fps) {
	frames = std::vector<int>();
	for (int i : list) {
		frames.push_back(i);
	}
	frames_per_second = fps;
	seconds_per_frame = 1.0f / fps;
	counter = 0.0f;
	current_index = 0;
	current_frame = frames[current_index];
}

void Anim::update(float time) {
	counter += time;
	if (counter >= seconds_per_frame) {
		counter = 0.0f;
		current_index = (current_index + 1) % frames.size();
		current_frame = frames[current_index];
	}
}
