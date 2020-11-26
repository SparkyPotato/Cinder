#include "PCH.h"

constexpr float ToDegrees(float radians)
{
	return radians * 180.f / Pi;
}

constexpr float ToRadians(float degrees)
{
	return degrees * Pi / 180.f;
}
