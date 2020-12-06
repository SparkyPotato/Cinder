#pragma once

class Normal;
class Matrix;

// Direction in 3D space, has a W coordinate of 0
class Vector
{
public:
	Vector() = default;
	Vector(float x, float y, float z);
	Vector(const Vector& other) = default;
	explicit operator Normal() const;

	Vector& operator=(const Vector& other);

	float operator[](uint8_t index) const;
	float& operator[](uint8_t index);

	Vector operator+(const Vector& other) const;
	Vector& operator+=(const Vector& other);

	Vector operator-() const;
	Vector operator-(const Vector& other) const;
	Vector& operator-=(const Vector& other);

	Vector operator*(const Matrix& matrix) const;
	Vector& operator*=(const Matrix& matrix);
	Vector operator*(float scalar) const;
	Vector& operator*=(float scalar);

	Vector operator/(float scalar) const;
	Vector& operator/=(float scalar);

	float GetLengthSquare() const;
	float GetLength() const;

	Vector GetNormalized() const;
	Vector& Normalize();

	bool IsNAN() const;

	// Whether this should be done or not is debatable
	inline float& X() { return reinterpret_cast<float*>(&m_Vector)[0]; }
	inline float& Y() { return reinterpret_cast<float*>(&m_Vector)[1]; }
	inline float& Z() { return reinterpret_cast<float*>(&m_Vector)[2]; }

	inline const float& X() const { return reinterpret_cast<const float*>(&m_Vector)[0]; }
	inline const float& Y() const { return reinterpret_cast<const float*>(&m_Vector)[1]; }
	inline const float& Z() const { return reinterpret_cast<const float*>(&m_Vector)[2]; }

private:
	friend class Point;

	friend float Dot(const Vector& first, const Vector& second);
	friend float Dot(const Normal& first, const Vector& second);
	friend float Dot(const Vector& first, const Normal& second);
	friend Vector Cross(const Vector& first, const Vector& second);

	Vector(const __m128& vector);

	__m128 m_Vector = _mm_set_ps(0.f, 0.f, 0.f, 0.f);
};

bool operator==(const Vector& first, const Vector& second);
bool operator!=(const Vector& first, const Vector& second);

Vector operator*(float scale, const Vector& direction);
float Dot(const Vector& first, const Vector& second);
// Only calculates a 3D cross product
Vector Cross(const Vector& first, const Vector& second);
void GenerateCoordinateSystem(const Vector& normalized, Vector& outFirst, Vector& outSecond);
Vector Lerp(const Vector& from, const Vector& to, float ratio);
Vector Shuffle(const Vector& direction, uint8_t x, uint8_t y, uint8_t z);

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

	Point operator+(const Vector& direction) const;
	Point& operator+=(const Vector& direction);

	Vector operator-(const Point& other) const;
	Point operator-(const Vector& direction) const;
	Point& operator-=(const Vector& direction);

	Point operator*(const Matrix& matrix) const;
	Point& operator*=(const Matrix& matrix);

	bool IsNAN() const;

	// Whether this should be done or not is debatable
	inline float& X() { return reinterpret_cast<float*>(&m_Vector)[0]; }
	inline float& Y() { return reinterpret_cast<float*>(&m_Vector)[1]; }
	inline float& Z() { return reinterpret_cast<float*>(&m_Vector)[2]; }

	inline const float& X() const { return reinterpret_cast<const float*>(&m_Vector)[0]; }
	inline const float& Y() const { return reinterpret_cast<const float*>(&m_Vector)[1]; }
	inline const float& Z() const { return reinterpret_cast<const float*>(&m_Vector)[2]; }

private:
	Point(const __m128& vector);

	__m128 m_Vector = _mm_set_ps(1.f, 0.f, 0.f, 0.f);
};

bool operator==(const Point& first, const Point& second);
bool operator!=(const Point& first, const Point& second);

float Distance(const Point& first, const Point& second);
float DistanceSquare(const Point& first, const Point& second);
Point Lerp(const Point& from, const Point& to, float ratio);
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
	explicit operator Vector() const;

	Normal& operator=(const Normal& other);

	float operator[](uint8_t index) const;
	float& operator[](uint8_t index);

	Normal operator+(const Normal& other) const;
	Normal& operator+=(const Normal& other);

	Normal operator-() const;
	Normal operator-(const Normal& other) const;
	Normal& operator-=(const Normal& other);

	Normal operator*(const Matrix& matrix) const;
	Normal& operator*=(const Matrix& matrix);
	Normal operator*(float scalar) const;
	Normal& operator*=(float scalar);

	Normal operator/(float scalar) const;
	Normal& operator/=(float scalar);

	float GetLengthSquare() const;
	float GetLength() const;

	Normal GetNormalized() const;
	Normal& Normalize();

	bool IsNAN() const;

	// Whether this should be done or not is debatable
	inline float& X() { return reinterpret_cast<float*>(&m_Vector)[0]; }
	inline float& Y() { return reinterpret_cast<float*>(&m_Vector)[1]; }
	inline float& Z() { return reinterpret_cast<float*>(&m_Vector)[2]; }

	inline const float& X() const { return reinterpret_cast<const float*>(&m_Vector)[0]; }
	inline const float& Y() const { return reinterpret_cast<const float*>(&m_Vector)[1]; }
	inline const float& Z() const { return reinterpret_cast<const float*>(&m_Vector)[2]; }

private:
	friend float Dot(const Normal& first, const Normal& second);
	friend float Dot(const Normal& first, const Vector& second);
	friend float Dot(const Vector& first, const Normal& second);

	Normal(const __m128& vector);

	__m128 m_Vector = _mm_set_ps(0.f, 0.f, 0.f, 0.f);
};


bool operator==(const Normal& first, const Normal& second);
bool operator!=(const Normal& first, const Normal& second);

float Dot(const Normal& first, const Normal& second);
float Dot(const Normal& first, const Vector& second);
float Dot(const Vector& first, const Normal& second);
Normal FlipAlong(const Normal& normal, const Vector& along);

template<>
struct fmt::formatter<Vector>
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
			"(X: {:" + ParseString + "}, Y: {:" + ParseString + "}, Z: {:" + ParseString + "})";

		return end;
	}

	template<typename FormatContext>
	auto format(const Vector& direction, FormatContext& context)
	{
		return format_to(
			context.out(),
			ParseString,
			direction.X(), direction.Y(), direction.Z()
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
			"(X: {:" + ParseString + "}, Y: {:" + ParseString + "}, Z: {:" + ParseString + "})";

		return end;
	}

	template<typename FormatContext>
	auto format(const Point& direction, FormatContext& context)
	{
		return format_to(
			context.out(),
			ParseString,
			direction.X(), direction.Y(), direction.Z()
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
			"(X: {:" + ParseString + "}, Y: {:" + ParseString + "}, Z: {:" + ParseString + "})";

		return end;
	}

	template<typename FormatContext>
	auto format(const Normal& direction, FormatContext& context)
	{
		return format_to(
			context.out(),
			ParseString,
			direction.X(), direction.Y(), direction.Z()
		);
	}
};
