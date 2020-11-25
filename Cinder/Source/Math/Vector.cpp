#include "PCH.h"

#include "Vector.h"

Direction::Direction(float x, float y, float z)
{
	// Reverse because it flips the order around:
	// Actually stored in memory as x|y|z|0.f
	m_Vector = _mm_set_ps(0.f, z, y, x);

	ASSERT(!IsNAN(), "Constructed direction is NaN!");
}

Direction::Direction(__m128 vector)
	: m_Vector(vector)
{}

float Direction::operator[](uint8_t index) const
{
	ASSERT(index < 3, "Direction index out of range!");

	return reinterpret_cast<const float*>(&m_Vector)[index];
}

float& Direction::operator[](uint8_t index)
{
	ASSERT(index < 3, "Direction index out of range!");

	return reinterpret_cast<float*>(&m_Vector)[index];
}

Direction& Direction::operator=(const Direction& other)
{
	m_Vector = other.m_Vector;

	return *this;
}

Direction Direction::operator+(const Direction& other) const
{
	return Direction(
		_mm_add_ps(m_Vector, other.m_Vector)
	);
}

Direction& Direction::operator+=(const Direction& other)
{
	m_Vector = _mm_add_ps(m_Vector, other.m_Vector);

	return *this;
}

Direction Direction::operator-() const
{
	return Direction(-X, -Y, -Z);
}

Direction Direction::operator-(const Direction& other) const
{
	return Direction(
		_mm_sub_ps(m_Vector, other.m_Vector)
	);
}

Direction& Direction::operator-=(const Direction& other)
{
	m_Vector = _mm_sub_ps(m_Vector, other.m_Vector);

	return *this;
}

Direction Direction::operator*(float scalar) const
{
	__m128 scale = _mm_set_ps(scalar, scalar, scalar, scalar);
	return Direction(
		_mm_mul_ps(m_Vector, scale)
	);
}

Direction& Direction::operator*=(float scalar)
{ 
	__m128 scale = _mm_set_ps(scalar, scalar, scalar, scalar);
	m_Vector = _mm_mul_ps(m_Vector, scale);
	
	return *this;
}

Direction Direction::operator/(float scalar) const
{
	__m128 scale = _mm_set_ps(scalar, scalar, scalar, scalar);
	return Direction(
		_mm_div_ps(m_Vector, scale)
	);
}

Direction& Direction::operator/=(float scalar)
{
	__m128 scale = _mm_set_ps(scalar, scalar, scalar, scalar);
	m_Vector = _mm_div_ps(m_Vector, scale);
	
	return *this;
}

float Direction::Dot(const Direction& first, const Direction& second)
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

Direction Direction::Cross(const Direction& first, const Direction& second)
{
	__m128 result = _mm_sub_ps(
		_mm_mul_ps(second.m_Vector, _mm_shuffle_ps(first.m_Vector, first.m_Vector, _MM_SHUFFLE(3, 0, 2, 1))),
		_mm_mul_ps(first.m_Vector, _mm_shuffle_ps(second.m_Vector, second.m_Vector, _MM_SHUFFLE(3, 0, 2, 1)))
	);
	
	return Direction(_mm_shuffle_ps(result, result, _MM_SHUFFLE(3, 0, 2, 1 )));
}

float Direction::GetLengthSquare() const
{
	return Direction::Dot(*this, *this);
}

float Direction::GetLength() const
{
	return sqrt(Direction::Dot(*this, *this));
}

Direction Direction::GetNormalized() const
{
	return *this / GetLength();
}

Direction& Direction::Normalize()
{
	*this /= GetLength();
	
	return *this;
}

bool Direction::IsNAN()
{
	return std::isnan(X) || std::isnan(Y) || std::isnan(Z);
}
