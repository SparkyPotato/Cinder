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
	ASSERT(m_Inverse == m_Matrix.Inverse(), "Provided inverse does not match with the calculated inverse!");
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
	float x = (*this)(Vector(1.f, 0.f, 0.f)).GetLengthSquare();
	float y = (*this)(Vector(0.f, 1.f, 0.f)).GetLengthSquare();
	float z = (*this)(Vector(0.f, 0.f, 1.f)).GetLengthSquare();
	
	return !(IsNearlyEqual(x, 1.f) || IsNearlyEqual(y, 1.f) || IsNearlyEqual(z, 1.f));
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
	return normal * m_Inverse.Transpose();
}

Ray Transform::operator()(const Ray& ray) const
{
	return Ray(ray.Origin * m_Matrix, ray.Direction * m_Matrix,
		ray.Extent);
}

Bound Transform::operator()(const Bound& bound) const
{
	const Transform& t = *this;
	Bound ret(t(Point(bound.Minimum.X, bound.Minimum.Y, bound.Minimum.Z)));
	ret = Union(ret, t(Point(bound.Maximum.X, bound.Minimum.Y, bound.Minimum.Z)));
	ret = Union(ret, t(Point(bound.Minimum.X, bound.Maximum.Y, bound.Minimum.Z)));
	ret = Union(ret, t(Point(bound.Minimum.X, bound.Minimum.Y, bound.Maximum.Z)));
	ret = Union(ret, t(Point(bound.Minimum.X, bound.Maximum.Y, bound.Maximum.Z)));
	ret = Union(ret, t(Point(bound.Maximum.X, bound.Maximum.Y, bound.Minimum.Z)));
	ret = Union(ret, t(Point(bound.Maximum.X, bound.Minimum.Y, bound.Maximum.Z)));
	ret = Union(ret, t(Point(bound.Maximum.X, bound.Maximum.Y, bound.Maximum.Z)));
	
	return ret;
}

Transform Transform::operator*(const Transform& transform) const
{
	return Transform(m_Matrix * transform.m_Matrix, transform.m_Inverse * m_Inverse);
}

Transform Transform::operator*=(const Transform& transform)
{
	*this = *this * transform;

	return *this;
}

bool Transform::ChangesHandedness()
{
	float determinant =
	m_Matrix[0][0] * (m_Matrix[1][1] * m_Matrix[2][2] - m_Matrix[1][2] * m_Matrix[2][1]) -
	m_Matrix[0][1] * (m_Matrix[1][0] * m_Matrix[2][2] - m_Matrix[1][2] * m_Matrix[2][0]) +
	m_Matrix[0][2] * (m_Matrix[1][0] * m_Matrix[2][1] - m_Matrix[1][1] * m_Matrix[2][0]);
	return determinant < 0;
}


Quaternion Transform::ToQuaternion()
{
	const Matrix& matrix = m_Matrix;
	Quaternion quat;
	float trace = matrix[0][0] + matrix[1][1] + matrix[2][2];

	if (trace > 0.f)
	{
		float s = std::sqrt(trace + 1.0f);
		quat.W = s / 2.0f;
		s = 0.5f / s;
		quat.X = (matrix[1][2] - matrix[2][1]) * s;
		quat.Y = (matrix[2][0] - matrix[0][2]) * s;
		quat.Z = (matrix[0][1] - matrix[1][0]) * s;
	}
	else
	{
		const int nxt[3] = { 1, 2, 0 };
		float q[3];
		int i = 0;
		if (matrix[1][1] > matrix[0][0]) { i = 1; }
		if (matrix[2][2] > matrix[i][i]) { i = 2; }

		int j = nxt[i];
		int k = nxt[j];
		float s = std::sqrt((matrix[i][i] - (matrix[j][j] + matrix[k][k])) + 1.0f);
		q[i] = s * 0.5f;
		if (s != 0.f) s = 0.5f / s;
		quat.W = (matrix[j][k] - matrix[k][j]) * s;
		q[j] = (matrix[i][j] + matrix[j][i]) * s;
		q[k] = (matrix[i][k] + matrix[k][i]) * s;

		quat.X = q[0];
		quat.Y = q[1];
		quat.Z = q[2];
	}

	return quat;
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
		1.f,     0.f,     0.f,     0.f,
		0.f,     1.f,     0.f,     0.f,
		0.f,     0.f,     1.f,     0.f,
		delta.X, delta.Y, delta.Z, 1.f
	);
	Matrix inverse(
		1.f,      0.f,      0.f,      0.f,
		0.f,      1.f,      0.f,      0.f,
		0.f,      0.f,      1.f,      0.f,
		-delta.X, -delta.Y, -delta.Z, 1.f
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
		1.f / scale, 0.f,         0.f,         0.f,
		0.f,         1.f / scale, 0.f,         0.f,
		0.f,         0.f,         1.f / scale, 0.f,
		0.f,         0.f,         0.f,         1.f
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
		1.f / scale.X, 0.f,           0.f,           0.f,
		0.f,           1.f / scale.Y, 0.f,           0.f,
		0.f,           0.f,           1.f / scale.Z, 0.f,
		0.f,           0.f,           0.f,           1.f
	);

	return Transform(matrix, inverse);
}

// https://en.wikipedia.org/wiki/Rotation_matrix
Transform Rotate(const Vector& eulerDegrees)
{
	float cosX = std::cos(ToRadians(eulerDegrees.X));
	float sinX = std::sin(ToRadians(eulerDegrees.X));
	float cosY = std::cos(ToRadians(eulerDegrees.Y));
	float sinY = std::sin(ToRadians(eulerDegrees.Y));
	float cosZ = std::cos(ToRadians(eulerDegrees.Z));
	float sinZ = std::sin(ToRadians(eulerDegrees.Z));
	
	Matrix matrix(
		cosY * cosZ,                      cosY * sinZ,                      -sinY,       0.f,
		sinX * sinY * cosZ - cosX * sinZ, sinX * sinY * sinZ + cosX * cosZ, sinX * cosY, 0.f,
		cosX * sinY * cosZ + sinX * sinZ, cosX * sinY * sinZ - sinX * cosZ, cosX * cosY, 0.f,
		0.f,                              0.f,                              0.f,         1.f
	);
	
	// Since a rotation matrix is orthogonal, its inverse is its transpose.
	// Transposing is much faster.
	return Transform(matrix, matrix.Transpose());
}

Transform Rotate(const Vector& axis, float degreeAngle)
{
	Vector normalized = axis.GetNormalized();
	float cos = std::cos(ToRadians(degreeAngle));
	float sin = std::sin(ToRadians(degreeAngle));
	float x = normalized.X;
	float y = normalized.Y;
	float z = normalized.Z;
	
	Matrix matrix(
		cos + x * x * (1.f - cos),     x * y * (1.f - cos) + z * sin, x * z * (1.f - cos) - y * sin, 0.f,
		x * y * (1.f - cos) - x * sin, cos + y * y * (1.f - cos),     y * z * (1.f - cos) + x * sin, 0.f,
		x * z * (1.f - cos) + y * sin, y * z * (1.f - cos) - x * sin, cos + z * z * (1.f - cos),     0.f,
		0.f,                           0.f,                           0.f,                           1.f
	);
	
	return Transform(matrix, matrix.Transpose());
}

Transform Rotate(const Quaternion& quaternion)
{
	float X = quaternion.X;
	float Y = quaternion.Y;
	float Z = quaternion.Z;
	float W = quaternion.W;

	Matrix matrix(
		1.f - 2.f * (Y * Y + Z * Z), 2.f * (X * Y + Z * W), 2.f * (X * Z - Y * W), 0.f,
		2.f * (X * Y - Z * W), 1.f - 2.f * (X * X + Z * Z), 2 * (Y * Z + X * W), 0.f,
		2.f * (X * Z + Y * W), 2.f * (Y * Z - X * W), 1.f - 2.f * (X * X + Y * Y), 0.f,
		0.f, 0.f, 0.f, 1.f
	);

	return Transform(matrix, matrix.Transpose());
}

Transform LookAt(const Point& location, const Point& focus, const Vector& up)
{
	Matrix matrix;
	matrix[0][3] = location.X;
	matrix[1][3] = location.Y;
	matrix[2][3] = location.Z;
	matrix[3][3] = 1.f;
	
	Vector direction = focus - location;
	Vector right = Cross(up.GetNormalized(), direction).Normalize();
	Vector newUp = 	Cross(direction, up);
	
	matrix[0][0] = right.X;
	matrix[1][0] = right.Y;
	matrix[2][0] = right.Z;
	matrix[3][0] = 0.f;
	
	matrix[0][1] = newUp.X;
	matrix[1][1] = newUp.Y;
	matrix[2][1] = newUp.Z;
	matrix[3][1] = 0.f;
	
	matrix[0][2] = direction.X;
	matrix[1][2] = direction.Y;
	matrix[2][2] = direction.Z;
	matrix[3][2] = 0.f;
	
	return Transform(matrix.Inverse(), matrix);
}

bool YAML::convert<Transform>::decode(const Node& node, Transform& transform)
{
	if (!node["Position"])
	{
		Error("Transform does not have Position!");
		return false;
	}
	if (!node["Rotation"])
	{
		Error("Transform does not have Rotation!");
		return false;
	}
	if (!node["Scale"])
	{
		Error("Transform does not have Scale!");
		return false;
	}

	if (!node["Position"].IsSequence() || node["Position"].size() != 3)
	{
		Error("Invalid format for Position!");
		return false;
	}
	if (!node["Rotation"].IsSequence() || node["Rotation"].size() != 3)
	{
		Error("Invalid format for Rotation!");
		return false;
	}
	if (!node["Scale"].IsSequence() || node["Scale"].size() != 3)
	{
		Error("Invalid format for Scale!");
		return false;
	}

	try 
	{ 
		Vector scale(node["Scale"][0].as<float>(), node["Scale"][1].as<float>(), node["Scale"][2].as<float>());
		transform = Scale(scale);
	}
	catch (YAML::Exception& e)
	{
		Error("Scale expects floats (line {})!", e.mark.line);
		return false;
	}

	try
	{
		Vector rotation(node["Rotation"][0].as<float>(), node["Rotation"][1].as<float>(), node["Rotation"][2].as<float>());
		transform *= Rotate(rotation);
	}
	catch (YAML::Exception& e)
	{
		Error("Rotation expects floats (line {})!", e.mark.line);
		return false;
	}

	try
	{
		Vector position(node["Position"][0].as<float>(), node["Position"][1].as<float>(), node["Position"][2].as<float>());
		transform *= Translate(position);
	}
	catch (YAML::Exception& e)
	{
		Error("Position expects floats (line {})!", e.mark.line);
		return false;
	}

	return true;
}
