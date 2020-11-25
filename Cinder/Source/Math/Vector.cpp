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
{
	ASSERT(!IsNAN(), "Constructed direction is NaN!");
}

Direction::operator Normal()
{
	return Normal(X, Y, Z);
}

Direction& Direction::operator=(const Direction& other)
{
	m_Vector = other.m_Vector;

	return *this;
}

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

Direction operator*(float scale, const Direction& direction)
{
	return direction * scale;
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

float Direction::GetLengthSquare() const
{
	return Dot(*this, *this);
}

float Direction::GetLength() const
{
	return std::sqrt(Dot(*this, *this));
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

float Dot(const Direction& first, const Direction& second)
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

Direction Cross(const Direction& first, const Direction& second)
{
	__m128 result = _mm_sub_ps(
		_mm_mul_ps(second.m_Vector, _mm_shuffle_ps(first.m_Vector, first.m_Vector, _MM_SHUFFLE(3, 0, 2, 1))),
		_mm_mul_ps(first.m_Vector, _mm_shuffle_ps(second.m_Vector, second.m_Vector, _MM_SHUFFLE(3, 0, 2, 1)))
	);

	return Direction(_mm_shuffle_ps(result, result, _MM_SHUFFLE(3, 0, 2, 1)));
}

void GenerateCoordinateSystem(const Direction& normalized, Direction& outFirst, Direction& outSecond)
{
	if (std::abs(normalized.X) > std::abs(normalized.Y))
	{
		outFirst = Direction(-normalized.Z, 0, normalized.X) /
			std::sqrt(normalized.X * normalized.X + normalized.Z * normalized.Z);
	}
	else
	{
		outSecond = Direction(0, normalized.Z, -normalized.Y) /
			std::sqrt(normalized.Y * normalized.Y + normalized.Z * normalized.Z);
	}
	outSecond = Cross(normalized, outFirst);
}

Direction Shuffle(const Direction& direction, uint8_t x, uint8_t y, uint8_t z)
{
	return Direction(direction[x], direction[y], direction[z]);
}

Point::Point(float x, float y, float z)
{
	m_Vector = _mm_set_ps(1.f, z, y, x);

	ASSERT(!IsNAN(), "Constructed point is NaN!");
}

Point::Point(__m128 vector)
	: m_Vector(vector)
{
	ASSERT(!IsNAN(), "Constructed point is NaN!");
}

Point& Point::operator=(const Point& other)
{
	m_Vector = other.m_Vector;

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

Point Point::operator+(const Direction& direction) const
{
	return Point(
		_mm_add_ps(m_Vector, direction.m_Vector)
	);
}

Point& Point::operator+=(const Direction& direction)
{
	m_Vector = _mm_add_ps(m_Vector, direction.m_Vector);

	return *this;
}

Direction Point::operator-(const Point& other) const
{
	return Direction(
		_mm_sub_ps(m_Vector, other.m_Vector)
	);
}

Point Point::operator-(const Direction& direction) const
{
	return Point(
		_mm_sub_ps(m_Vector, direction.m_Vector)
	);
}

Point& Point::operator-=(const Direction& direction)
{
	m_Vector = _mm_sub_ps(m_Vector, direction.m_Vector);

	return *this;
}

float Distance(const Point& first, const Point& second)
{
	return (first - second).GetLength();
}

float DistanceSquare(const Point& first, const Point& second)
{
	return (first - second).GetLengthSquare();
}

Point Lerp(const Point& from, const Point& to, float value)
{
	return Point(
		from + (to - from) * value
	);
}

Point Min(const Point& first, const Point& second)
{
	return Point(
		std::min(first.X, second.X),
		std::min(first.Y, second.Y),
		std::min(first.Z, second.Z)
	);
}

Point Max(const Point& first, const Point& second)
{
	return Point(
		std::max(first.X, second.X),
		std::max(first.Y, second.Y),
		std::max(first.Z, second.Z)
	);
}

bool Point::IsNAN()
{
	return std::isnan(X) || std::isnan(Y) || std::isnan(Z);
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

Normal::Normal(__m128 vector)
	: m_Vector(vector)
{
	ASSERT(!IsNAN(), "Constructed normal is NaN!");
}

Normal::operator Direction()
{
	return Direction(X, Y, Z);
}

Normal& Normal::operator=(const Normal& other)
{
	m_Vector = other.m_Vector;

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
	return Normal(-X, -Y, -Z);
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

bool Normal::IsNAN()
{
	return std::isnan(X) || std::isnan(Y) || std::isnan(Z);
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

float Dot(const Normal& first, const Direction& second)
{
	__m128 mul = _mm_mul_ps(first.m_Vector, second.m_Vector);
	__m128 shuffle = _mm_shuffle_ps(mul, mul, _MM_SHUFFLE(2, 3, 0, 1));
	__m128 sums = _mm_add_ps(mul, shuffle);
	shuffle = _mm_movehl_ps(shuffle, sums);
	sums = _mm_add_ss(sums, shuffle);
	return _mm_cvtss_f32(sums);
}

float Dot(const Direction& first, const Normal& second)
{
	__m128 mul = _mm_mul_ps(first.m_Vector, second.m_Vector);
	__m128 shuffle = _mm_shuffle_ps(mul, mul, _MM_SHUFFLE(2, 3, 0, 1));
	__m128 sums = _mm_add_ps(mul, shuffle);
	shuffle = _mm_movehl_ps(shuffle, sums);
	sums = _mm_add_ss(sums, shuffle);
	return _mm_cvtss_f32(sums);
}

Normal FlipAlong(const Normal& normal, const Direction& along)
{
	return (Dot(normal, along) < 0.f) ? -normal : normal;
}
