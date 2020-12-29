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
#include "Core/Math/Vector.h"

#include "Core/Math/Matrix.h"

Vector::Vector(float x, float y, float z)
{
	// Reverse because it flips the order around:
	// Actually stored in memory as x|y|z|0.f
	m_Vector = _mm_set_ps(0.f, z, y, x);

	ASSERT(!IsNAN(), "Constructed direction is NaN!");
}

Vector::Vector(const __m128& vector)
	: m_Vector(vector)
{
	ASSERT(!IsNAN(), "Constructed direction is NaN!");
}

Vector::operator Normal() const
{
	return Normal(X(), Y(), Z());
}

Vector& Vector::operator=(const Vector& other)
{
	memcpy(&m_Vector, &other.m_Vector, sizeof(__m128));

	return *this;
}

float Vector::operator[](uint8_t index) const
{
	ASSERT(index < 3, "Direction index out of range!");

	return reinterpret_cast<const float*>(&m_Vector)[index];
}

float& Vector::operator[](uint8_t index)
{
	ASSERT(index < 3, "Direction index out of range!");

	return reinterpret_cast<float*>(&m_Vector)[index];
}

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
	return Vector(-X(), -Y(), -Z());
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

Vector Vector::operator*(const Matrix& matrix) const
{
	float vec[3];
	for (int i = 0; i < 3; i++)
	{
		__m128 mul = _mm_mul_ps(m_Vector, matrix.m_Columns[i]);
		__m128 shuffle = _mm_shuffle_ps(mul, mul, _MM_SHUFFLE(2, 3, 0, 1));
		__m128 sums = _mm_add_ps(mul, shuffle);
		shuffle = _mm_movehl_ps(shuffle, sums);
		sums = _mm_add_ss(sums, shuffle);
		vec[i] =  _mm_cvtss_f32(sums);
	}
	
	return Vector(vec[0], vec[1], vec[2]);
}

Vector& Vector::operator*=(const Matrix& matrix)
{
	*this = *this * matrix;
	
	return *this;
}

Vector operator*(float scale, const Vector& direction)
{
	return direction * scale;
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

float Vector::GetLengthSquare() const
{
	return Dot(*this, *this);
}

float Vector::GetLength() const
{
	return std::sqrt(Dot(*this, *this));
}

Vector Vector::GetNormalized() const
{
	return *this / GetLength();
}

Vector Vector::TransformTo(const Vector& x, const Vector& y, const Vector& z) const
{
	return Vector(
		Dot(*this, x),
		Dot(*this, y),
		Dot(*this, z)
	);
}

Vector Vector::TransformFrom(const Vector& x, const Vector& y, const Vector& z) const
{
	return X() * x +
		Y() * y +
		Z() * z;
}

Vector& Vector::Normalize()
{
	*this /= GetLength();
	
	return *this;
}

Vector Vector::GetAbs() const
{
	return Vector(std::abs(X()), std::abs(Y()), std::abs(Z()));
}

uint8_t Vector::GetMaxDimension() const
{
	return (X() > Y()) ? ((X() > Z()) ? 0 : 2) :
		((Y() > Z()) ? 1 : 2);
}

bool Vector::IsNAN() const
{
	return std::isnan(X()) || std::isnan(Y()) || std::isnan(Z());
}

bool operator==(const Vector& first, const Vector& second)
{
	return first.X() == second.X() &&
		first.Y() == second.Y() &&
		first.Z() == second.Z();
}

bool operator!=(const Vector& first, const Vector& second)
{
	return !(first == second);
}

float Dot(const Vector& first, const Vector& second)
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

Vector Hadamard(const Vector& first, const Vector& second)
{
	return Vector(_mm_mul_ps(first.m_Vector, second.m_Vector));
}

Vector Cross(const Vector& first, const Vector& second)
{
	__m128 result = _mm_sub_ps(
		_mm_mul_ps(second.m_Vector, _mm_shuffle_ps(first.m_Vector, first.m_Vector, _MM_SHUFFLE(3, 0, 2, 1))),
		_mm_mul_ps(first.m_Vector, _mm_shuffle_ps(second.m_Vector, second.m_Vector, _MM_SHUFFLE(3, 0, 2, 1)))
	);

	return Vector(_mm_shuffle_ps(result, result, _MM_SHUFFLE(3, 0, 2, 1)));
}

void GenerateCoordinateSystem(const Vector& normalized, Vector& outFirst, Vector& outSecond)
{
	if (std::abs(normalized.X()) > std::abs(normalized.Y()))
	{
		outFirst = Vector(-normalized.Z(), 0, normalized.X()) /
			std::sqrt(normalized.X() * normalized.X() + normalized.Z() * normalized.Z());
	}
	else
	{
		outFirst = Vector(0, normalized.Z(), -normalized.Y()) /
			std::sqrt(normalized.Y() * normalized.Y() + normalized.Z() * normalized.Z());
	}
	outSecond = Cross(normalized, outFirst);
}

Vector Lerp(const Vector& from, const Vector& to, float ratio)
{
	return Vector(
		from + (to - from) * ratio
	);
}

Vector Shuffle(const Vector& direction, uint8_t x, uint8_t y, uint8_t z)
{
	return Vector(direction[x], direction[y], direction[z]);
}

Vector Max(const Vector& first, const Vector& second)
{
	return Vector(_mm_max_ps(first.m_Vector, second.m_Vector));
}

Vector Min(const Vector& first, const Vector& second)
{
	return Vector(_mm_min_ps(first.m_Vector, second.m_Vector));
}

Vector SphericalDirection(float sin, float cos, float phi)
{
	return Vector(sin * std::cos(phi), cos, cos * std::sin(phi));
}

Vector SphericalDirection(float sin, float cos, float phi, const Vector& x, const Vector& y, const Vector& z)
{
	return sin * std::cos(phi) * x + cos * y + sin * std::sin(phi) * z;
}

Point::Point(float x, float y, float z)
{
	m_Vector = _mm_set_ps(1.f, z, y, x);

	ASSERT(!IsNAN(), "Constructed point is NaN!");
}

Point::Point(const __m128& vector)
	: m_Vector(vector)
{
	ASSERT(!IsNAN(), "Constructed point is NaN!");
}

Point& Point::operator=(const Point& other)
{
	memcpy(&m_Vector, &other.m_Vector, sizeof(__m128));

	return *this;
}

float Point::operator[](uint8_t index) const
{
	ASSERT(index < 3, "Point index out of range!");

	return reinterpret_cast<const float*>(&m_Vector)[index];
}

float& Point::operator[](uint8_t index)
{
	ASSERT(index < 3, "Point index out of range!");

	return reinterpret_cast<float*>(&m_Vector)[index];
}

Point Point::operator+(const Vector& direction) const
{
	return Point(
		_mm_add_ps(m_Vector, direction.m_Vector)
	);
}

Point& Point::operator+=(const Vector& direction)
{
	m_Vector = _mm_add_ps(m_Vector, direction.m_Vector);

	return *this;
}

Vector Point::operator-(const Point& other) const
{
	return Vector(
		_mm_sub_ps(m_Vector, other.m_Vector)
	);
}

Point Point::operator-(const Vector& direction) const
{
	return Point(
		_mm_sub_ps(m_Vector, direction.m_Vector)
	);
}

Point& Point::operator-=(const Vector& direction)
{
	m_Vector = _mm_sub_ps(m_Vector, direction.m_Vector);

	return *this;
}

Point Point::operator*(const Matrix& matrix) const
{
	float vec[4];
	for (int i = 0; i < 4; i++)
	{
		__m128 mul = _mm_mul_ps(m_Vector, matrix.m_Columns[i]);
		__m128 shuffle = _mm_shuffle_ps(mul, mul, _MM_SHUFFLE(2, 3, 0, 1));
		__m128 sums = _mm_add_ps(mul, shuffle);
		shuffle = _mm_movehl_ps(shuffle, sums);
		sums = _mm_add_ss(sums, shuffle);
		vec[i] =  _mm_cvtss_f32(sums);
	}
	
	__m128 point;
	point = _mm_load_ps(vec);
	
	if (vec[3] != 1.f)
	{
		__m128 scale = _mm_set_ps(vec[3], vec[3], vec[3], vec[3]);
		point = _mm_div_ps(point, scale);
	}
	
	return Point(point);
}

Point& Point::operator*=(const Matrix& matrix)
{
	*this = *this * matrix;
	
	return *this;
}

bool operator==(const Point& first, const Point& second)
{
	return first.X() == second.X() &&
		first.Y() == second.Y() &&
		first.Z() == second.Z();
}

bool operator!=(const Point& first, const Point& second)
{
	return !(first == second);
}

float Distance(const Point& first, const Point& second)
{
	return (first - second).GetLength();
}

float DistanceSquare(const Point& first, const Point& second)
{
	return (first - second).GetLengthSquare();
}

Point Lerp(const Point& from, const Point& to, float ratio)
{
	return Point(
		from + (to - from) * ratio
	);
}

Point Min(const Point& first, const Point& second)
{
	return Point(
		std::min(first.X(), second.X()),
		std::min(first.Y(), second.Y()),
		std::min(first.Z(), second.Z())
	);
}

Point Max(const Point& first, const Point& second)
{
	return Point(
		std::max(first.X(), second.X()),
		std::max(first.Y(), second.Y()),
		std::max(first.Z(), second.Z())
	);
}

bool Point::IsNAN() const
{
	return std::isnan(X()) || std::isnan(Y()) || std::isnan(Z());
}

Point Shuffle(const Point& point, uint8_t x, uint8_t y, uint8_t z)
{
	return Point(point[x], point[y], point[z]);
}

Normal::Normal(float x, float y, float z)
{
	m_Vector = _mm_set_ps(0.f, z, y, x);

	ASSERT(!IsNAN(), "Constructed normal is NaN!");
}

Normal::Normal(const __m128& vector)
	: m_Vector(vector)
{
	ASSERT(!IsNAN(), "Constructed normal is NaN!");
}

Normal::operator Vector() const
{
	return Vector(X(), Y(), Z());
}

Normal& Normal::operator=(const Normal& other)
{
	memcpy(&m_Vector, &other.m_Vector, sizeof(__m128));

	return *this;
}

float Normal::operator[](uint8_t index) const
{
	ASSERT(index < 3, "Normal index out of range!");

	return reinterpret_cast<const float*>(&m_Vector)[index];
}

float& Normal::operator[](uint8_t index)
{
	ASSERT(index < 3, "Normal index out of range!");

	return reinterpret_cast<float*>(&m_Vector)[index];
}

Normal Normal::operator+(const Normal& other) const
{
	return Normal(
		_mm_add_ps(m_Vector, other.m_Vector)
	);
}

Normal& Normal::operator+=(const Normal& other)
{
	m_Vector = _mm_add_ps(m_Vector, other.m_Vector);

	return *this;
}

Normal Normal::operator-() const
{
	return Normal(-X(), -Y(), -Z());
}

Normal Normal::operator-(const Normal& other) const
{
	return Normal(
		_mm_sub_ps(m_Vector, other.m_Vector)
	);
}

Normal& Normal::operator-=(const Normal& other)
{
	m_Vector = _mm_sub_ps(m_Vector, other.m_Vector);

	return *this;
}

Normal Normal::operator*(const Matrix& matrix) const
{
	float vec[3];
	for (int i = 0; i < 3; i++)
	{
		__m128 mul = _mm_mul_ps(m_Vector, matrix.m_Columns[i]);
		__m128 shuffle = _mm_shuffle_ps(mul, mul, _MM_SHUFFLE(2, 3, 0, 1));
		__m128 sums = _mm_add_ps(mul, shuffle);
		shuffle = _mm_movehl_ps(shuffle, sums);
		sums = _mm_add_ss(sums, shuffle);
		vec[i] =  _mm_cvtss_f32(sums);
	}
	
	return Normal(vec[0], vec[1], vec[2]);
}

Normal& Normal::operator*=(const Matrix& matrix)
{
	*this = *this * matrix;
	
	return *this;
}

Normal Normal::operator*(float scalar) const
{
	__m128 scale = _mm_set_ps(scalar, scalar, scalar, scalar);
	return Normal(
		_mm_mul_ps(m_Vector, scale)
	);
}

Normal& Normal::operator*=(float scalar)
{
	__m128 scale = _mm_set_ps(scalar, scalar, scalar, scalar);
	m_Vector = _mm_mul_ps(m_Vector, scale);

	return *this;
}

Normal Normal::operator/(float scalar) const
{
	__m128 scale = _mm_set_ps(scalar, scalar, scalar, scalar);
	return Normal(
		_mm_div_ps(m_Vector, scale)
	);
}

Normal& Normal::operator/=(float scalar)
{
	__m128 scale = _mm_set_ps(scalar, scalar, scalar, scalar);
	m_Vector = _mm_div_ps(m_Vector, scale);

	return *this;
}

float Normal::GetLengthSquare() const
{
	return Dot(*this, *this);
}

float Normal::GetLength() const
{
	return std::sqrt(Dot(*this, *this));
}

Normal Normal::GetNormalized() const
{
	return *this / GetLength();
}

Normal& Normal::Normalize()
{
	*this /= GetLength();

	return *this;
}

bool Normal::IsNAN() const
{
	return std::isnan(X()) || std::isnan(Y()) || std::isnan(Z());
}

bool operator==(const Normal& first, const Normal& second)
{
	return first.X() == second.X() &&
		first.Y() == second.Y() &&
		first.Z() == second.Z();
}

bool operator!=(const Normal& first, const Normal& second)
{
	return !(first == second);
}

float Dot(const Normal& first, const Normal& second)
{
	__m128 mul = _mm_mul_ps(first.m_Vector, second.m_Vector);
	__m128 shuffle = _mm_shuffle_ps(mul, mul, _MM_SHUFFLE(2, 3, 0, 1));
	__m128 sums = _mm_add_ps(mul, shuffle);
	shuffle = _mm_movehl_ps(shuffle, sums);
	sums = _mm_add_ss(sums, shuffle);
	return _mm_cvtss_f32(sums);
}

float Dot(const Normal& first, const Vector& second)
{
	__m128 mul = _mm_mul_ps(first.m_Vector, second.m_Vector);
	__m128 shuffle = _mm_shuffle_ps(mul, mul, _MM_SHUFFLE(2, 3, 0, 1));
	__m128 sums = _mm_add_ps(mul, shuffle);
	shuffle = _mm_movehl_ps(shuffle, sums);
	sums = _mm_add_ss(sums, shuffle);
	return _mm_cvtss_f32(sums);
}

float Dot(const Vector& first, const Normal& second)
{
	__m128 mul = _mm_mul_ps(first.m_Vector, second.m_Vector);
	__m128 shuffle = _mm_shuffle_ps(mul, mul, _MM_SHUFFLE(2, 3, 0, 1));
	__m128 sums = _mm_add_ps(mul, shuffle);
	shuffle = _mm_movehl_ps(shuffle, sums);
	sums = _mm_add_ss(sums, shuffle);
	return _mm_cvtss_f32(sums);
}

Normal FlipAlong(const Normal& normal, const Vector& along)
{
	return (Dot(normal, along) < 0.f) ? -normal : normal;
}
