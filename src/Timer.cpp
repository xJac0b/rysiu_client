#include "pch.h"
#include "Timer.h"

Timer::Timer(float start, float max) : value(start), max(max)
{
}

Timer::~Timer()
{
}

void Timer::reset()
{
	this->value = 0.f;
}

void Timer::update(const float& dt)
{
	if (this->value < this->max)
		this->value += 100.f * dt;
}

const bool& Timer::get()
{
	if (this->value >= this->max)
	{
		this->value = 0.f;
		return true;
	}

	return false;
}
