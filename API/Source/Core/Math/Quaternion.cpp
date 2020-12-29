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
#include "Core/Math/Quaternion.h"


Quaternion::Quaternion(float x, float y, float z, float w)
{
	m_Vector = _mm_set_ps(w, z, y, x);
}

Quaternion::Quaternion(const __m128& vector)
	: m_Vector(vector)
{}

Quaternion Quaternion::operator+(const Quaternion& other) const
{
	return _mm_add_ps(m_Vector, other.m_Vector);
}

Quaternion& Quaternion::operator+=(const Quaternion& other)
{
	m_Vector = _mm_add_ps(m_Vector, other.m_Vector);

	return *this;
}

Quaternion Quaternion::operator-(const Quaternion& other) const
{
	return _mm_sub_ps(m_Vector, other.m_Vector);
}

Quaternion& Quaternion::operator-=(const Quaternion& other)
{
	m_Vector = _mm_sub_ps(m_Vector, other.m_Vector);

	return *this;
}

Quaternion Quaternion::operator*(float scalar) const
{
	__m128 scale = _mm_set_ps(scalar, scalar, scalar, scalar);

	return _mm_mul_ps(m_Vector, scale);
}

Quaternion& Quaternion::operator*=(float scalar)
{
	__m128 scale = _mm_set_ps(scalar, scalar, scalar, scalar);
	m_Vector = _mm_mul_ps(m_Vector, scale);

	return *this;
}

Quaternion Quaternion::operator/(float scalar) const
{
	__m128 scale = _mm_set_ps(scalar, scalar, scalar, scalar);

	return _mm_div_ps(m_Vector, scale);
}

Quaternion& Quaternion::operator/=(float scalar)
{
	__m128 scale = _mm_set_ps(scalar, scalar, scalar, scalar);
	m_Vector = _mm_div_ps(m_Vector, scale);

	return *this;
}

Quaternion& Quaternion::Normalize()
{
	*this /= Dot(*this, *this);

	return *this;
}

Quaternion Quaternion::GetNormalized() const
{
	return *this / Dot(*this, *this);
}

Quaternion& Quaternion::operator=(const Quaternion& other)
{
	m_Vector = other.m_Vector;

	return *this;
}

Quaternion operator*(float scalar, const Quaternion& quaternion)
{
	__m128 scale = _mm_set_ps(scalar, scalar, scalar, scalar);

	return _mm_mul_ps(quaternion.m_Vector, scale);
}

Quaternion Slerp(const Quaternion& from, const Quaternion& to, float ratio)
{
	ASSERT(IsNearlyEqual(Dot(from, from), 1.f) && IsNearlyEqual(Dot(to, to), 1.f), 
		"Input quaternions are not normalized!");

	float cos = Dot(from, to);

	if (cos > .9995f) { return ((1 - ratio) * from + ratio * to).Normalize(); }
	else 
	{
		float theta = std::acos(cos);
		float thetap = theta * ratio;
		Quaternion qperp = (to - from * cos).Normalize();
		return from * std::cos(thetap) + qperp * std::sin(thetap);
	}
}

float Dot(const Quaternion& first, const Quaternion& second)
{
	__m128 mul = _mm_mul_ps(first.m_Vector, second.m_Vector);
	__m128 shuffle = _mm_shuffle_ps(mul, mul, _MM_SHUFFLE(2, 3, 0, 1));
	__m128 sums = _mm_add_ps(mul, shuffle);
	shuffle = _mm_movehl_ps(shuffle, sums);
	sums = _mm_add_ss(sums, shuffle);
	return _mm_cvtss_f32(sums);
}
