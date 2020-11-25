#pragma once

// Portable SIMD intrinsics
#include <immintrin.h>

class Normal;

// Direction in 3D space, has a W coordinate of 0
class Direction
{
public:
	Direction() = default;
	Direction(float x, float y, float z);
	Direction(const Direction& other) = default;
	explicit operator Normal();

	Direction& operator=(const Direction& other);

	float operator[](uint8_t index) const;
	float& operator[](uint8_t index);

	Direction operator+(const Direction& other) const;
	Direction& operator+=(const Direction& other);

	Direction operator-() const;
	Direction operator-(const Direction& other) const;
	Direction& operator-=(const Direction& other);

	Direction operator*(float scalar) const;
	Direction& operator*=(float scalar);
	
	Direction operator/(float scalar) const;
	Direction& operator/=(float scalar);
	
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
	friend class Point;
	friend float Dot(const Direction& first, const Direction& second);
	friend float Dot(const Normal& first, const Direction& second);
	friend float Dot(const Direction& first, const Normal& second);
	friend Direction Cross(const Direction& first, const Direction& second);

	Direction(__m128 vector);

	__m128 m_Vector = _mm_set_ps(0.f, 0.f, 0.f, 0.f);
};

Direction operator*(float scale, const Direction& direction);
float Dot(const Direction& first, const Direction& second);
// Only calculates a 3D cross product
Direction Cross(const Direction& first, const Direction& second);
void GenerateCoordinateSystem(const Direction& normalized, Direction& outFirst, Direction& outSecond);
Direction Shuffle(const Direction& direction, uint8_t x, uint8_t y, uint8_t z);

// Point in 3D space, with W coordinate of 1
class Point
{
public:
	Point() = default;
	Point(float x, float y, float z);
	Point(const Point& other) = default;

	Point& operator=(const Point& other);

	float operator[](uint8_t index) const;
	float& operator[](uint8_t index);

	Point operator+(const Direction& direction) const;
	Point& operator+=(const Direction& direction);

	Direction operator-(const Point& other) const;
	Point operator-(const Direction& direction) const;
	Point& operator-=(const Direction& direction);

	bool IsNAN();

	// Whether this should be done or not is debatable
	float& X = reinterpret_cast<float*>(&m_Vector)[0];
	float& Y = reinterpret_cast<float*>(&m_Vector)[1];
	float& Z = reinterpret_cast<float*>(&m_Vector)[2];

private:
	Point(__m128 vector);

	__m128 m_Vector = _mm_set_ps(1.f, 0.f, 0.f, 0.f);
};

float Distance(const Point& first, const Point& second);
float DistanceSquare(const Point& first, const Point& second);
Point Lerp(const Point& from, const Point& to, float value);
Point Min(const Point& first, const Point& second);
Point Max(const Point& first, const Point& second);
Point Shuffle(const Point& point, uint8_t x, uint8_t y, uint8_t z);

// A surface normal. MAY NOT be normalized
class Normal
{
public:
	Normal() = default;
	Normal(float x, float y, float z);
	Normal(const Normal & other) = default;
	explicit operator Direction();

	Normal& operator=(const Normal& other);

	float operator[](uint8_t index) const;
	float& operator[](uint8_t index);

	Normal operator+(const Normal& other) const;
	Normal& operator+=(const Normal& other);

	Normal operator-() const;
	Normal operator-(const Normal& other) const;
	Normal& operator-=(const Normal& other);

	Normal operator*(float scalar) const;
	Normal& operator*=(float scalar);

	Normal operator/(float scalar) const;
	Normal& operator/=(float scalar);

	float GetLengthSquare() const;
	float GetLength() const;

	Normal GetNormalized() const;
	Normal& Normalize();

	bool IsNAN();

	// Whether this should be done or not is debatable
	float& X = reinterpret_cast<float*>(&m_Vector)[0];
	float& Y = reinterpret_cast<float*>(&m_Vector)[1];
	float& Z = reinterpret_cast<float*>(&m_Vector)[2];

private:
	friend float Dot(const Normal& first, const Normal& second);
	friend float Dot(const Normal& first, const Direction& second);
	friend float Dot(const Direction& first, const Normal& second);

	Normal(__m128 vector);

	__m128 m_Vector = _mm_set_ps(0.f, 0.f, 0.f, 0.f);
};

float Dot(const Normal& first, const Normal& second);
float Dot(const Normal& first, const Direction& second);
float Dot(const Direction& first, const Normal& second);
Normal FlipAlong(const Normal& normal, const Direction& along);

template<>
struct fmt::formatter<Direction>
{
	std::string ParseString;

	auto parse(format_parse_context& context)
	{
		auto end = std::find(context.begin(), context.end(), '}');

		if (end != context.end())
		{
			ParseString = std::string(context.begin(), end);
		}

		ParseString =
			"[X: {:" + ParseString + "}, Y: {:" + ParseString + "}, Z: {:" + ParseString + "}]";

		return end;
	}

	template<typename FormatContext>
	auto format(const Direction& direction, FormatContext& context)
	{
		return format_to(
			context.out(),
			ParseString,
			direction.X, direction.Y, direction.Z
		);
	}
};

template<>
struct fmt::formatter<Point>
{
	std::string ParseString;

	auto parse(format_parse_context& context)
	{
		auto end = std::find(context.begin(), context.end(), '}');

		if (end != context.end())
		{
			ParseString = std::string(context.begin(), end);
		}

		ParseString =
			"[X: {:" + ParseString + "}, Y: {:" + ParseString + "}, Z: {:" + ParseString + "}]";

		return end;
	}

	template<typename FormatContext>
	auto format(const Point& direction, FormatContext& context)
	{
		return format_to(
			context.out(),
			ParseString,
			direction.X, direction.Y, direction.Z
		);
	}
};

template<>
struct fmt::formatter<Normal>
{
	std::string ParseString;

	auto parse(format_parse_context& context)
	{
		auto end = std::find(context.begin(), context.end(), '}');

		if (end != context.end())
		{
			ParseString = std::string(context.begin(), end);
		}

		ParseString =
			"[X: {:" + ParseString + "}, Y: {:" + ParseString + "}, Z: {:" + ParseString + "}]";

		return end;
	}

	template<typename FormatContext>
	auto format(const Normal& direction, FormatContext& context)
	{
		return format_to(
			context.out(),
			ParseString,
			direction.X, direction.Y, direction.Z
		);
	}
};
