#pragma once

#include "Transform.h"

class Quaternion
{
public:
	Quaternion() = default;
	Quaternion(float x, float y, float z, float w);
	Quaternion(const Quaternion& other) = default;
	Quaternion(const Transform& transform);

	Quaternion& operator=(const Quaternion& other);

	Quaternion operator+(const Quaternion& other) const;
	Quaternion& operator+=(const Quaternion& other);

	Quaternion operator-(const Quaternion& other) const;
	Quaternion& operator-=(const Quaternion& other);

	Quaternion operator*(float scalar) const;
	Quaternion& operator*=(float scalar);

	Quaternion operator/(float scalar) const;
	Quaternion& operator/=(float scalar);

	Transform ToTransform() const;

	Quaternion& Normalize();
	Quaternion GetNormalized() const;

	float& X = reinterpret_cast<float*>(&m_Vector)[0];
	float& Y = reinterpret_cast<float*>(&m_Vector)[1];
	float& Z = reinterpret_cast<float*>(&m_Vector)[2];
	float& W = reinterpret_cast<float*>(&m_Vector)[3];

private:
	Quaternion(const __m128& vector);

	friend Quaternion operator*(float scalar, const Quaternion& quaternion);
	friend float Dot(const Quaternion& first, const Quaternion& second);

	__m128 m_Vector = _mm_set_ps(1.f, 0.f, 0.f, 0.f);
};

Quaternion operator*(float scalar, const Quaternion& quaternion);

// Both quaternions MUST BE normalized
Quaternion Slerp(const Quaternion& from, const Quaternion& to, float ratio);
float Dot(const Quaternion& first, const Quaternion& second);

template<>
struct fmt::formatter<Quaternion>
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
			"(X: {:" + ParseString + "}, Y: {:" + ParseString + "}, Z: {:" + ParseString + "}, W: {" + ParseString + "})";

		return end;
	}

	template<typename FormatContext>
	auto format(const Quaternion& quaternion, FormatContext& context)
	{
		return format_to(
			context.out(),
			ParseString,
			quaternion.X, quaternion.Y, quaternion.Z, quaternion.W
		);
	}
};
