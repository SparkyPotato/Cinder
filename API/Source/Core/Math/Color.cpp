//    Copyright 2021 SparkyPotato
//
//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
//    
//    http://www.apache.org/licenses/LICENSE-2.0
//    
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.

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

float Color::Y() const 
{
	constexpr float YWeight[3] = { 0.212671f, 0.715160f, 0.072169f };

	return YWeight[0] * R() + YWeight[1] * G() + YWeight[2] * B();
}

bool operator==(const Color& first, const Color& second)
{
	return first.R() == second.R() && first.G() == second.G() && first.B() == second.B();
}

bool operator!=(const Color& first, const Color& second)
{
	return !(first == second);
}

bool operator<(const Color& first, const Color& second)
{
	return first.R() < second.R() && first.G() < second.G() && first.B() < second.B();
}

bool operator<=(const Color& first, const Color& second)
{
	return first.R() <= second.R()&& first.G() <= second.G()&& first.B() <= second.B();
}

bool operator>(const Color& first, const Color& second)
{
	return !(first <= second);
}

bool operator>=(const Color& first, const Color& second)
{
	return !(first < second);
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
		color.R() = node[0].as<float>();
		color.G() = node[1].as<float>();
		color.B() = node[2].as<float>();
	}
	catch (YAML::Exception& e)
	{
		Error("Color values must be float (line {})!", e.mark.line + 1);
		return false;
	}
	
	return true;
}
