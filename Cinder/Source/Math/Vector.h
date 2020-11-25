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
	Vector& operator*=(float scalar);
	
	Vector operator/(float scalar) const;
	Vector& operator/=(float scalar);
	
	static float Dot(const Vector& first, const Vector& second);
	// Only calculates a 3D cross product
	static Vector Cross(const Vector& first, const Vector& second);
	
	float GetLengthSquare() const;
	float GetLength() const;
	
	Vector GetNormalized() const;
	Vector& Normalize();

	// Whether this should be done or not is debatable
	float& X = reinterpret_cast<float*>(&m_Vector)[0];
	float& Y = reinterpret_cast<float*>(&m_Vector)[1];
	float& Z = reinterpret_cast<float*>(&m_Vector)[2];
	float& W = reinterpret_cast<float*>(&m_Vector)[3];

private:
	Vector(__m128 vector);

	__m128 m_Vector = _mm_set_ps(0.f, 0.f, 0.f, 0.f);
};
