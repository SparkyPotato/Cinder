#include "Math/Vector.h"

Vector& Vector::operator=(const Vector& other)
{
	X = other.X;
	Y = other.Y;
	Z = other.Z;

	return *this;
}

Vector Vector::operator+(const Vector other) const
{
	return Vector(
		X + other.X,
		Y + other.Y,
		Z + other.Z
	);
}

Vector& Vector::operator+=(const Vector other)
{
	X += other.X;
	Y += other.Y;
	Z += other.Z;

	return *this;
}

Vector Vector::operator-(const Vector other) const
{
	return Vector(
		X - other.X,
		Y - other.Y,
		Z - other.Z
	);
}

Vector& Vector::operator-=(const Vector other)
{
	X -= other.X;
	Y -= other.Y;
	Z -= other.Z;

	return *this;
}

Vector Vector::operator-() const
{
	return Vector(-X, -Y, -Z);
}

Vector Vector::operator*(float scale) const
{
	return Vector(
		X * scale,
		Y * scale,
		Z * scale
	);
}

Vector& Vector::operator*=(float scale)
{
	X *= scale;
	Y *= scale;
	Z *= scale;

	return *this;
}

Vector Vector::operator/(float scale) const
{
	return Vector(
		X / scale,
		Y / scale,
		Z / scale
	);
}

Vector& Vector::operator/=(float scale)
{
	X /= scale;
	Y /= scale;
	Z /= scale;

	return *this;
}

Vector& Vector::Normalize()
{
	*this /= Length();
	return *this;
}

float Vector::Dot(const Vector& first, const Vector& second)
{
	return first.X * second.X +
		first.Y * second.Y +
		first.Z * second.Z;
}

Vector Vector::Cross(const Vector& first, const Vector& second)
{
	float x, y, z;
	x = first.Y * second.Z - first.Z * second.Y;
	y = first.Z * second.X - first.X * second.Z;
	z = first.X * second.Y - first.Y * second.X;

	return Vector(x, y, z);
}

Vector Vector::Lerp(const Vector& first, const Vector& second, float factor)
{
	return Vector(
		(second - first) * factor + first
	);
}

void from_json(const nlohmann::json& j, Vector& vector)
{
	vector.X = j.at("X").get<float>();
	vector.Y = j.at("Y").get<float>();
	vector.Z = j.at("Z").get<float>();
}

void to_json(nlohmann::json& j, const Vector& vector)
{
	j["X"] = vector.X;
	j["Y"] = vector.Y;
	j["Z"] = vector.Z;
}
