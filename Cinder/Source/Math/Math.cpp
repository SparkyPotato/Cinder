#include "PCH.h"

constexpr float ToDegrees(float radians)
{
	return radians * 180.f / Pi;
}

constexpr float ToRadians(float degrees)
{
	return degrees * Pi / 180.f;
}

float Lerp(float from, float to, float ratio)
{
	return from + (to - from) * ratio;
}

float IsNearlyEqual(float value, float equalTo, float tolerance)
{
	return value <= equalTo + tolerance && value >= equalTo - tolerance;
}
