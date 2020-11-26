#include "PCH.h"

#include "Transform.h"

Transform::Transform(float matrix[4][4])
	: m_Matrix(matrix)
{
	m_Inverse = m_Matrix.Inverse();
}

Transform::Transform(const Matrix& matrix)
	: m_Matrix(matrix), m_Inverse(matrix.Inverse())
{}

Transform::Transform(const Matrix& matrix, const Matrix& inverse)
	: m_Matrix(matrix), m_Inverse(inverse)
{
	ASSERT(m_Inverse == m_Matrix.Inverse(), "Provided inverse does not match with calculated inverse!");
}

Transform Transform::GetInverse() const
{
	return Transform(m_Inverse, m_Matrix);
}

Transform Transform::GetTranspose() const
{
	return Transform(m_Matrix.Transpose(), m_Inverse.Transpose());
}

bool Transform::IsIdentity() const
{
	return m_Matrix == Matrix();
}

bool Transform::HasScale() const
{
	float lengthX = m_Matrix.GetValue(0, 0);
	float lengthY = m_Matrix.GetValue(1, 1);
	float lengthZ = m_Matrix.GetValue(2, 2);

	return !(IsNearlyEqual(lengthX, 1.f) || IsNearlyEqual(lengthY, 1.f) || IsNearlyEqual(lengthZ, 1.f));
}

Point Transform::operator()(const Point& point) const
{
	return point * m_Matrix;
}

Vector Transform::operator()(const Vector& vector) const
{
	return vector * m_Matrix;
}

Normal Transform::operator()(const Normal& normal) const
{
	return normal * m_Matrix;
}

Ray Transform::operator()(const Ray& ray) const
{
	return Ray(ray.Origin * m_Matrix, ray.Direction * m_Matrix,
		ray.Extent, ray.CurrentMedium);
}

RayDifferential Transform::operator()(const RayDifferential& ray) const
{
	return RayDifferential(ray.Origin * m_Matrix, ray.Direction * m_Matrix,
		ray.Extent, ray.CurrentMedium);
}

Bound Transform::operator()(const Bound& bound) const
{
	return Bound(bound.Minimum * m_Matrix, bound.Maximum * m_Matrix);
}

Transform Transform::operator*(const Transform& transform) const
{
	return Transform(transform.m_Matrix * m_Matrix, transform.m_Inverse * m_Inverse);
}

bool operator==(const Transform& first, const Transform& second)
{
	return first.m_Matrix == second.m_Matrix;
}

bool operator!=(const Transform& first, const Transform& second)
{
	return !(first == second);
}

Transform Translate(const Vector& delta)
{
	Matrix matrix(
		1.f, 0.f, 0.f, delta.X,
		0.f, 1.f, 0.f, delta.Y,
		0.f, 0.f, 1.f, delta.Z,
		0.f, 0.f, 0.f, 1.f
	);
	Matrix inverse(
		1.f, 0.f, 0.f, -delta.X,
		0.f, 1.f, 0.f, -delta.Y,
		0.f, 0.f, 1.f, -delta.Z,
		0.f, 0.f, 0.f, 1.f
	);

	return Transform(matrix, inverse);
}

Transform Scale(float scale)
{
	Matrix matrix(
		scale, 0.f,   0.f,   0.f,
		0.f,   scale, 0.f,   0.f,
		0.f,   0.f,   scale, 0.f,
		0.f,   0.f,   0.f,   1.f
	);
	Matrix inverse(
		-scale, 0.f,    0.f,    0.f,
		0.f,    -scale, 0.f,    0.f,
		0.f,    0.f,    -scale, 0.f,
		0.f,    0.f,    0.f,    1.f
	);

	return Transform(matrix, inverse);
}

Transform Scale(const Vector& scale)
{
	Matrix matrix(
		scale.X, 0.f,     0.f,     0.f,
		0.f,     scale.Y, 0.f,     0.f,
		0.f,     0.f,     scale.Z, 0.f,
		0.f,     0.f,     0.f,     1.f
	);
	Matrix inverse(
		-scale.X, 0.f,      0.f,      0.f,
		0.f,      -scale.Y, 0.f,      0.f,
		0.f,      0.f,      -scale.Z, 0.f,
		0.f,      0.f,      0.f,      1.f
	);

	return Transform(matrix, inverse);
}

Transform Rotate(const Vector& eulerDegrees)
{

}

Transform Rotate(const Vector& axis, float degreeAngle)
{

}

Transform LookAt(const Point& location, const Point& focus, const Vector& up)
{

}