#pragma once
class Timer
{
public:
	Timer(float start, float max);
	~Timer();

	void reset();
	void update(const float& dt);
	const bool& get();
private:
	float value;
	float max;
};
