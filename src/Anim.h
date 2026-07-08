#pragma once

#include <vector>
#include <initializer_list>

class Anim {
public:
	std::vector<int> frames;
	float frames_per_second;
	float seconds_per_frame;
	float counter;
	int current_frame;
	int current_index;
	Anim();
	Anim(std::initializer_list<int> list, float fps);
	void update(float time);
};
