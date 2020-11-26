#pragma once

#include <cmath>

inline constexpr float Pi = 3.14159265359f;
inline constexpr float InversePi = 0.31830988618f;
inline constexpr float FloatMax = std::numeric_limits<float>::max();
inline constexpr float FloatMin = std::numeric_limits<float>::lowest();

constexpr float ToDegrees(float radians);
constexpr float ToRadians(float degrees);

float Lerp(float from, float to, float ratio);
