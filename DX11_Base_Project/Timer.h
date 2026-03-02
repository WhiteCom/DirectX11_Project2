#pragma once

#include <chrono>

class Timer
{
public:
	Timer() noexcept;
	~Timer() noexcept;

	float Mark() noexcept;
	float Peek() noexcept;
	void Reset() noexcept;

public:
	std::chrono::steady_clock::time_point last;
};