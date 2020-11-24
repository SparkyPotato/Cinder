#include "PCH.h"

#include "Vector.h"

Vector::Vector(float x, float y, float z, float w)
{
	// Reverse because it flips the order around:
	// Actually stored in memory as x|y|z|w
	m_Vector = _mm_set_ps(w, z, y, x);
}

Vector::Vector(__m128 vector)
	: m_Vector(vector)
{}

Vector Vector::operator+(const Vector& other) const
{
	return Vector(
		_mm_add_ps(m_Vector, other.m_Vector)
	);
}

Vector& Vector::operator+=(const Vector& other)
{
	m_Vector = _mm_add_ps(m_Vector, other.m_Vector);

	return *this;
}

Vector Vector::operator-() const
{
	return Vector(-X, -Y, -Z, -W);
}

Vector Vector::operator-(const Vector& other) const
{
	return Vector(
		_mm_sub_ps(m_Vector, other.m_Vector)
	);
}

Vector& Vector::operator-=(const Vector& other)
{
	m_Vector = _mm_sub_ps(m_Vector, other.m_Vector);

	return *this;
}

Vector Vector::operator*(float scalar) const
{
	__m128 scale = _mm_set_ps(scalar, scalar, scalar, scalar);

	return Vector(
		_mm_mul_ps(m_Vector, scale)
	);
}
