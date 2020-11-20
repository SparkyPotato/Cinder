#pragma once

#include <cmath>

#include "json/json.hpp"

class Vector
{
public:
	Vector() = default;

	Vector(float x, float y, float z)
		: X(x), Y(y), Z(z)
	{}

	Vector(const Vector& other) = default;

	Vector& operator=(const Vector& other);

	Vector operator+(const Vector other) const;
	Vector& operator+=(const Vector other);

	Vector operator-() const;

	Vector operator-(const Vector other) const;
	Vector& operator-=(const Vector other);

	Vector operator*(float scale) const;
	Vector& operator*=(float scale);

	Vector operator/(float scale) const;
	Vector& operator/=(float scale);

	inline float Length() const { return std::sqrt(X * X + Y * Y + Z * Z); }

	Vector& Normalize();

	static float Dot(const Vector& first, const Vector& second);
	static Vector Cross(const Vector& first, const Vector& second);

	static Vector Lerp(const Vector& first, const Vector& second, float factor);

public:
	float X = 0, Y = 0, Z = 0;
	float& R = X;
	float& G = Y;
	float& B = Z;
};

using Color = Vector;

void from_json(const nlohmann::json& j, Vector& vector);
void to_json(nlohmann::json& j, const Vector& vector);
