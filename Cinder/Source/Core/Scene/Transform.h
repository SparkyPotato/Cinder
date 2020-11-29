#pragma once

#include "Core/Math/Bound.h"
#include "Core/Math/Matrix.h"
#include "Core/Math/Quaternion.h"
#include "Core/Math/Vector.h"
#include "Ray.h"

// Transform of an object at an instant in time
class Transform
{
public:
	Transform() = default;
	Transform(float matrix[4][4]);
	Transform(const Matrix& matrix);
	Transform(const Matrix& matrix, const Matrix& inverse);

	Transform GetInverse() const;
	Transform GetTranspose() const;

	bool IsIdentity() const;

	const Matrix& GetMatrix() const { return m_Matrix; }
	const Matrix& GetInverseMatrix() const { return m_Inverse; }

	bool HasScale() const;

	Point operator()(const Point& point) const;
	Vector operator()(const Vector& vector) const;
	Normal operator()(const Normal& normal) const;
	Ray operator()(const Ray& ray) const;
	RayDifferential operator()(const RayDifferential& ray) const;
	Bound operator()(const Bound& bound) const;
	Transform operator*(const Transform& transform) const;
	Transform operator*=(const Transform& transform);
	
	bool ChangesHandedness();

	Quaternion ToQuaternion();

private:
	friend bool operator==(const Transform& first, const Transform& second);

	Matrix m_Matrix, m_Inverse;
};

bool operator==(const Transform& first, const Transform& second);
bool operator!=(const Transform& first, const Transform& second);

Transform Translate(const Vector& delta);
Transform Scale(float scale);
Transform Scale(const Vector& scale);
Transform Rotate(const Vector& eulerDegrees);
Transform Rotate(const Quaternion& quaternion);
Transform Rotate(const Vector& axis, float degreeAngle);
Transform LookAt(const Point& location, const Point& focus, const Vector& up);

template<>
struct YAML::convert<Transform>
{
	static bool decode(const Node& node, Transform& transform);
};
