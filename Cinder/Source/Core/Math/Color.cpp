#include "PCH.h"
#include "Core/Math/Color.h"

Color::Color(float g)
{
	m_Vector = _mm_set_ps(0.f, g, g, g);
}

Color::Color(float r, float g, float b)
{
	m_Vector = _mm_set_ps(0.f, b, g, r);
}

Color::Color(__m128 vector)
{
	m_Vector = vector;
}

Color& Color::operator=(const Color& other)
{
	m_Vector = other.m_Vector;
	
	return *this;
}

Color Color::operator+(const Color& other) const
{
	return _mm_add_ps(m_Vector, other.m_Vector);
}

Color& Color::operator+=(const Color& other)
{
	m_Vector = _mm_add_ps(m_Vector, other.m_Vector);

	return *this;
}

Color Color::operator-(const Color& other) const
{
	return _mm_sub_ps(m_Vector, other.m_Vector);
}

Color& Color::operator-=(const Color& other)
{
	m_Vector = _mm_sub_ps(m_Vector, other.m_Vector);

	return *this;
}

Color Color::operator*(const Color& other) const
{
	return _mm_mul_ps(m_Vector, other.m_Vector);
}

Color& Color::operator*=(const Color& other)
{
	m_Vector = _mm_mul_ps(m_Vector, other.m_Vector);

	return *this;
}

Color Color::operator/(const Color& other) const
{
	return _mm_div_ps(m_Vector, other.m_Vector);
}

Color& Color::operator/=(const Color& other)
{
	m_Vector = _mm_div_ps(m_Vector, other.m_Vector);

	return *this;
}

Color Lerp(const Color& from, const Color& to, float ratio)
{
	return from + (to - from) * Color(ratio, ratio, ratio);
}

bool YAML::convert<Color>::decode(const Node& node, Color& color)
{
	if (!node.IsSequence())
	{
		Error("Color must be a sequence (line {})!", node.Mark().line + 1);
		return false;
	}
	if (node.size() != 3)
	{
		Error("Color must be 3 dimensional (line {)!", node.Mark().line + 1);
		return false;
	}
	
	try
	{
		color.R = node[0].as<float>();
		color.G = node[1].as<float>();
		color.B = node[2].as<float>();
	}
	catch (YAML::Exception& e)
	{
		Error("Color values must be float (line {})!", e.mark.line + 1);
		return false;
	}
	
	return true;
}
