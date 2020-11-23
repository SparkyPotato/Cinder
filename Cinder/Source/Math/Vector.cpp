#include "Vector.h"

#include "Globals.h"
#include "Math.h"
#include "Matrix.h"

Vector::Vector()
{
	X = Y = Z = 0.f;
	W = 1.f;
}

Vector::Vector(float all)
{
	X = Y = Z = W = all;
}

Vector::Vector(float xv, float yv, float zv)
{
	X = xv;
	Y = yv;
	Z = zv;
	W = 1.f;
}

Vector::Vector(float xv, float yv, float zv, float wv)
{
	X = xv;
	Y = yv;
	Z = zv;
	W = wv;
}

Vector::Vector(float values[3])
{
	X = values[0];
	Y = values[1];
	Z = values[2];
	W = 1.f;
}

Vector Vector::operator+(const Vector& other) const
{
	return Vector(
		X + other.X, 
		Y + other.Y, 
		Z + other.Z
	);
}

Vector Vector::operator+(float all) const
{
	return Vector(
		X + all, 
		Y + all, 
		Z + all
	);
}

Vector& Vector::operator+=(const Vector& other)
{
	X += other.X;
	Y += other.Y;
	Z += other.Z;

	return *this;
}

Vector& Vector::operator+=(float all)
{
	X += all;
	Y += all;
	Z += all;

	return *this;
}

Vector Vector::operator-() const
{
	return Vector(-X, -Y, -Z);
}

Vector Vector::operator-(const Vector& other) const
{
	return Vector(
		X - other.X, 
		Y - other.Y, 
		Z - other.Z
	);
}

Vector Vector::operator-(float all) const
{
	return Vector(
		X - all, 
		Y - all, 
		Z - all
	);
}

Vector& Vector::operator-=(const Vector& other)
{
	X -= other.X;
	Y -= other.Y;
	Z -= other.Z;

	return *this;
}

Vector& Vector::operator-=(float all)
{
	X -= all;
	Y -= all;
	Z -= all;

	return *this;
}

Vector Vector::operator*(float scalar) const
{
	return Vector(X * scalar, Y * scalar, Z * scalar);
}

Vector& Vector::operator*=(float scalar)
{
	X *= scalar;
	Y *= scalar;
	Z *= scalar;

	return *this;
}

Vector Vector::operator*(const Matrix& transform) const
{
	Vector vec;

	vec.X = X * transform.columns[0].X + Y * transform.columns[0].Y + Z * transform.columns[0].Z + W * transform.columns[0].W;
	vec.Y = X * transform.columns[1].X + Y * transform.columns[1].Y + Z * transform.columns[1].Z + W * transform.columns[1].W;
	vec.Z = X * transform.columns[2].X + Y * transform.columns[2].Y + Z * transform.columns[2].Z + W * transform.columns[2].W;
	vec.W = X * transform.columns[3].X + Y * transform.columns[3].Y + Z * transform.columns[3].Z + W * transform.columns[3].W;

	return vec;
}

Vector& Vector::operator*=(const Matrix& transform)
{
	*this = *this * transform;

	return *this;
}

Vector Vector::operator/(float scalar) const
{
	return Vector(X / scalar, Y / scalar, Z / scalar);
}

Vector& Vector::operator/=(float scalar)
{
	X /= scalar;
	Y /= scalar;
	Z /= scalar;

	return *this;
}

float Vector::GetLength() const
{
	float square = X * X + Y * Y + Z * Z;
	return std::sqrt(square);
}

Vector& Vector::Normalize()
{
	*this /= GetLength();
	return *this;
}

Vector Vector::GetNormalized() const
{
	return *this / GetLength();
}

float Vector::Dot3D(const Vector& first, const Vector& second)
{
	float dot = 0.f;

	dot += first.X * second.X;
	dot += first.Y * second.Y;
	dot += first.Z * second.Z;

	return dot;
}

Vector Vector::Cross3D(const Vector& first, const Vector& second)
{
	Vector vec;

	vec.X = (first.Y * second.Z) - (first.Z * second.Y);
	vec.Y = (first.Z * second.X) - (first.X * second.Z);
	vec.Z = (first.X * second.Y) - (first.Y * second.X);

	return vec;
}

Vector Vector::Hadamard3D(const Vector& first, const Vector& second)
{
	Vector vec;

	vec.X = first.X * second.X;
	vec.Y = first.Y * second.Y;
	vec.Z = first.Z * second.Z;

	return vec;
}

float Vector::Dot4D(const Vector& first, const Vector& second)
{
	float dot = 0.f;

	dot += first.X * second.X;
	dot += first.Y * second.Y;
	dot += first.Z * second.Z;
	dot += first.W * second.W;

	return dot;
}

Vector Vector::Hadamard4D(const Vector& first, const Vector& second)
{
	Vector vec;

	vec.X = first.X * second.X;
	vec.Y = first.Y * second.Y;
	vec.Z = first.Z * second.Z;
	vec.W = first.W * second.W;

	return vec;
}

namespace YAML
{
bool convert<Vector>::decode(const Node& node, Vector& vector)
{
	if (!node["X"])
	{
		Error("Vector does not have X! ("
			COLOR, "line ", node.Mark().line, "\x1b[31m).");
	}
	if (!node["Y"])
	{
		Error("Vector does not have Y! ("
			COLOR, "line ", node.Mark().line, "\x1b[31m).");
	}
	if (!node["Z"])
	{
		Error("Vector does not have Z! ("
			COLOR, "line ", node.Mark().line, "\x1b[31m).");
	}

	vector = Vector(
		node["X"].as<float>(),
		node["Y"].as<float>(),
		node["Z"].as<float>()
	);

	return true;
}
}
