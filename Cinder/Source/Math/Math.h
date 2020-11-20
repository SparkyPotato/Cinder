#pragma once

#include <cmath>

constexpr float PI = 3.1415926f;

inline float ToRadians(float degrees)
{
	return degrees * PI / 180.f;
}

inline float ToDegrees(float radians)
{
	return radians * 180.f / PI;
}
