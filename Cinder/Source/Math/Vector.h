#pragma once

// Portable SIMD intrinsics
#include <immintrin.h>

// Do not use directly, use Point or Direction instead
class Vector
{
public:
	Vector() = default;
	Vector(float x, float y, float z, float w);
	Vector(const Vector& other) = default;

	Vector operator+(const Vector& other) const;
	Vector& operator+=(const Vector& other);

	Vector operator-() const;
	Vector operator-(const Vector& other) const;
	Vector& operator-=(const Vector& other);

	Vector operator*(float scalar) const;

	// Whether this should be done or not is debatable
	float& X = reinterpret_cast<float*>(&m_Vector)[0];
	float& Y = reinterpret_cast<float*>(&m_Vector)[1];
	float& Z = reinterpret_cast<float*>(&m_Vector)[2];
	float& W = reinterpret_cast<float*>(&m_Vector)[3];

private:
	Vector(__m128 vector);

	__m128 m_Vector = _mm_set_ps(0.f, 0.f, 0.f, 0.f);
};
