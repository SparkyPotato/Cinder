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

Vector& Vector::operator*=(float scalar)
{ 
	__m128 scale = _mm_set_ps(scalar, scalar, scalar, scalar);
	m_Vector = _mm_mul_ps(m_Vector, scale);
	
	return *this;
}

Vector Vector::operator/(float scalar) const
{
	__m128 scale = _mm_set_ps(scalar, scalar, scalar, scalar);
	return Vector(
		_mm_div_ps(m_Vector, scale)
	);
}

Vector& Vector::operator/=(float scalar)
{
	__m128 scale = _mm_set_ps(scalar, scalar, scalar, scalar);
	m_Vector = _mm_div_ps(m_Vector, scale);
	
	return *this;
}

float Vector::Dot(const Vector& first, const Vector& second)
{
	// _mm_dot_ps is not used to retain support for all x64 processors.
	// It is also slower on Ryzen.
	__m128 mul = _mm_mul_ps(first.m_Vector, second.m_Vector);
	__m128 shuffle = _mm_shuffle_ps(mul, mul, _MM_SHUFFLE(2, 3, 0, 1));
	__m128 sums = _mm_add_ps(mul, shuffle);
	shuffle = _mm_movehl_ps(shuffle, sums);
	sums = _mm_add_ss(sums, shuffle);
	return _mm_cvtss_f32(sums);
}

Vector Vector::Cross(const Vector &first, const Vector &second)
{
	__m128 result = _mm_sub_ps(
		_mm_mul_ps(second.m_Vector, _mm_shuffle_ps(first.m_Vector, first.m_Vector, _MM_SHUFFLE(3, 0, 2, 1))),
		_mm_mul_ps(first.m_Vector, _mm_shuffle_ps(second.m_Vector, second.m_Vector, _MM_SHUFFLE(3, 0, 2, 1)))
	);
	
	return Vector(_mm_shuffle_ps(result, result, _MM_SHUFFLE(3, 0, 2, 1 )));
}

float Vector::GetLengthSquare() const
{
	return Vector::Dot(*this, *this);
}

float Vector::GetLength() const
{
	return sqrt(Vector::Dot(*this, *this));
}

Vector Vector::GetNormalized() const
{
	return *this / GetLength();
}

Vector& Vector::Normalize()
{
	*this /= GetLength();
	
	return *this;
}
