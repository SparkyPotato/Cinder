#pragma once

// Portable SIMD intrinsics
#include <immintrin.h>

// Direction in 3D space, has a W coordinate of 0
class Direction
{
public:
	Direction() = default;
	Direction(float x, float y, float z);
	Direction(const Direction& other) = default;

	float operator[](uint8_t index) const;
	float& operator[](uint8_t index);

	Direction& operator=(const Direction& other);

	Direction operator+(const Direction& other) const;
	Direction& operator+=(const Direction& other);

	Direction operator-() const;
	Direction operator-(const Direction& other) const;
	Direction& operator-=(const Direction& other);

	Direction operator*(float scalar) const;
	Direction& operator*=(float scalar);
	
	Direction operator/(float scalar) const;
	Direction& operator/=(float scalar);
	
	static float Dot(const Direction& first, const Direction& second);
	// Only calculates a 3D cross product
	static Direction Cross(const Direction& first, const Direction& second);
	
	float GetLengthSquare() const;
	float GetLength() const;
	
	Direction GetNormalized() const;
	Direction& Normalize();

	bool IsNAN();

	// Whether this should be done or not is debatable
	float& X = reinterpret_cast<float*>(&m_Vector)[0];
	float& Y = reinterpret_cast<float*>(&m_Vector)[1];
	float& Z = reinterpret_cast<float*>(&m_Vector)[2];

private:
	Direction(__m128 vector);

	__m128 m_Vector = _mm_set_ps(0.f, 0.f, 0.f, 0.f);
};
