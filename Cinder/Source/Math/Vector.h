#pragma once

#include "yaml-cpp/yaml.h"

class Matrix;

class Vector
{
public:
	Vector();
	Vector(float all);
	Vector(float x, float y, float z);
	Vector(float x, float y, float z, float w);
	Vector(float values[3]);
	Vector(const Vector& other) = default;

	Vector operator+(const Vector& other) const;
	Vector operator+(float all) const;
	Vector& operator+=(const Vector& other);
	Vector& operator+=(float all);

	Vector operator-() const;
	Vector operator-(const Vector& other) const;
	Vector operator-(float all) const;
	Vector& operator-=(const Vector& other);
	Vector& operator-=(float all);

	Vector operator*(float scalar) const;
	Vector& operator*=(float scalar);
	Vector operator*(const Matrix& transform) const;
	Vector& operator*=(const Matrix& transform);

	Vector operator/(float scalar) const;
	Vector& operator/=(float scalar);

	float GetLength() const;

	Vector& Normalize();
	Vector GetNormalized() const;

	static float Dot3D(const Vector& first, const Vector& second);
	static Vector Cross3D(const Vector& first, const Vector& second);
	static Vector Hadamard3D(const Vector& first, const Vector& second);

	static float Dot4D(const Vector& first, const Vector& second);
	static Vector Hadamard4D(const Vector& first, const Vector& second);

	static Vector Lerp(const Vector& from, const Vector& to, float value);

	float X;
	float Y;
	float Z;
	float W;
};

namespace YAML
{
template<>
struct convert<Vector>
{
	static bool decode(const Node& node, Vector& vector);
};
}
