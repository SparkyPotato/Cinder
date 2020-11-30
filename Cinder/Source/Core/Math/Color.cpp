#include "PCH.h"
#include "Color.h"

Color::Color(float r, float g, float b)
{
	m_Vector = _mm_set_ps(0.f, b, g, r);
}

Color::Color(__m128 vector)
{
	m_Vector = vector;
}

Color Color::operator+(const Color& other)
{
	return _mm_add_ps(m_Vector, other.m_Vector);
}

Color& Color::operator+=(const Color& other)
{
	m_Vector = _mm_add_ps(m_Vector, other.m_Vector);

	return *this;
}

Color Color::operator-(const Color& other)
{
	return _mm_sub_ps(m_Vector, other.m_Vector);
}

Color& Color::operator-=(const Color& other)
{
	m_Vector = _mm_sub_ps(m_Vector, other.m_Vector);

	return *this;
}

Color Color::operator*(const Color& other)
{
	return _mm_mul_ps(m_Vector, other.m_Vector);
}

Color& Color::operator*=(const Color& other)
{
	m_Vector = _mm_mul_ps(m_Vector, other.m_Vector);

	return *this;
}

Color Color::operator/(const Color& other)
{
	return _mm_div_ps(m_Vector, other.m_Vector);
}

Color& Color::operator/=(const Color& other)
{
	m_Vector = _mm_div_ps(m_Vector, other.m_Vector);

	return *this;
}

