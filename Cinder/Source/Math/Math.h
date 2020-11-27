#pragma once

#include <cmath>
#include <cstdint>

inline constexpr float Pi = 3.14159265359f;
inline constexpr float InversePi = 0.31830988618f;
inline constexpr float FloatMax = std::numeric_limits<float>::max();
inline constexpr float FloatMin = std::numeric_limits<float>::lowest();

constexpr float ToDegrees(float radians)
{
	return radians * 180.f / Pi;
}

constexpr float ToRadians(float degrees)
{
	return degrees * Pi / 180.f;
}

float Lerp(float from, float to, float ratio);

float IsNearlyEqual(float value, float equalTo, float tolerance = 0.001f);
