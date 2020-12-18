#pragma once

#include <cmath>
#include <cstdint>

inline constexpr float Pi = 3.14159265359f;
inline constexpr float InversePi = 0.31830988618f;
inline constexpr float FloatMax = std::numeric_limits<float>::max();
inline constexpr float FloatMin = std::numeric_limits<float>::lowest();
inline constexpr float FloatEpsilon = std::numeric_limits<float>::min();

inline constexpr float ToDegrees(float radians)
{
	return radians * 180.f / Pi;
}

inline constexpr float ToRadians(float degrees)
{
	return degrees * Pi / 180.f;
}

float Lerp(float from, float to, float ratio);

bool IsNearlyEqual(float value, float equalTo, float tolerance = 0.001f);

// coA = x^2 coefficient, coB = x coefficient, coC = constant
// outA <= outB
bool SolveQuadratic(float coA, float coB, float coC, float& outA, float& outB);
bool TestQuadratic(float coA, float coB, float coC);
