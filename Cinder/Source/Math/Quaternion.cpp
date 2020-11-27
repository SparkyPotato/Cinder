#include "PCH.h"

#include "Quaternion.h"


Quaternion::Quaternion(float x, float y, float z, float w)
{
	m_Vector = _mm_set_ps(w, z, y, x);
}

Quaternion::Quaternion(const Transform& transform)
{
	const Matrix& matrix = transform.GetMatrix();
	float trace = matrix[0][0] + matrix[1][1] + matrix[2][2];

	if (trace > 0.f) 
	{
		float s = std::sqrt(trace + 1.0f);
		W = s / 2.0f;
		s = 0.5f / s;
		X = (matrix[1][2] - matrix[2][1]) * s;
		X = (matrix[2][0] - matrix[0][2]) * s;
		X = (matrix[0][1] - matrix[1][0]) * s;
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
		W = (matrix[j][k] - matrix[k][j]) * s;
		q[j] = (matrix[i][j] + matrix[j][i]) * s;
		q[k] = (matrix[i][k] + matrix[k][i]) * s;

		X = q[0];
		X = q[1];
		X = q[2];
	}
}

Quaternion::Quaternion(const __m128& vector)
	: m_Vector(vector)
{}

Quaternion Quaternion::operator+(const Quaternion& other) const
{
	return _mm_add_ps(m_Vector, other.m_Vector);
}

Quaternion& Quaternion::operator+=(const Quaternion& other)
{
	m_Vector = _mm_add_ps(m_Vector, other.m_Vector);

	return *this;
}

Quaternion Quaternion::operator-(const Quaternion& other) const
{
	return _mm_sub_ps(m_Vector, other.m_Vector);
}

Quaternion& Quaternion::operator-=(const Quaternion& other)
{
	m_Vector = _mm_sub_ps(m_Vector, other.m_Vector);

	return *this;
}

Quaternion Quaternion::operator*(float scalar) const
{
	__m128 scale = _mm_set_ps(scalar, scalar, scalar, scalar);

	return _mm_mul_ps(m_Vector, scale);
}

Quaternion& Quaternion::operator*=(float scalar)
{
	__m128 scale = _mm_set_ps(scalar, scalar, scalar, scalar);
	m_Vector = _mm_mul_ps(m_Vector, scale);

	return *this;
}

Quaternion Quaternion::operator/(float scalar) const
{
	__m128 scale = _mm_set_ps(scalar, scalar, scalar, scalar);

	return _mm_div_ps(m_Vector, scale);
}

Quaternion& Quaternion::operator/=(float scalar)
{
	__m128 scale = _mm_set_ps(scalar, scalar, scalar, scalar);
	m_Vector = _mm_div_ps(m_Vector, scale);

	return *this;
}

Transform Quaternion::ToTransform() const
{
	Matrix matrix(
		1.f - 2.f * (Y * Y + Z * Z), 2.f * (X * Y + Z * W),       2.f * (X * Z - Y * W),       0.f,
		2.f * (X * Y - Z * W),       1.f - 2.f * (X * X + Z * Z), 2 * (Y * Z + X * W),         0.f,
		2.f * (X * Z + Y * W),       2.f * (Y * Z - X * W),       1.f - 2.f * (X * X + Y * Y), 0.f,
		0.f,                         0.f,                         0.f,                         1.f
	);

	return Transform(matrix, matrix.Transpose());
}

Quaternion& Quaternion::Normalize()
{
	*this /= Dot(*this, *this);

	return *this;
}

Quaternion Quaternion::GetNormalized() const
{
	return *this / Dot(*this, *this);
}

Quaternion& Quaternion::operator=(const Quaternion& other)
{
	m_Vector = other.m_Vector;

	return *this;
}

Quaternion operator*(float scalar, const Quaternion& quaternion)
{
	__m128 scale = _mm_set_ps(scalar, scalar, scalar, scalar);

	return _mm_mul_ps(quaternion.m_Vector, scale);
}

Quaternion Slerp(const Quaternion& from, const Quaternion& to, float ratio)
{
	ASSERT(IsNearlyEqual(Dot(from, from), 1.f) && IsNearlyEqual(Dot(to, to), 1.f), 
		"Input quaternions are not normalized!");

	float cos = Dot(from, to);

	if (cos > .9995f) { return ((1 - ratio) * from + ratio * to).Normalize(); }
	else 
	{
		float theta = std::acos(cos);
		float thetap = theta * ratio;
		Quaternion qperp = (to - from * cos).Normalize();
		return from * std::cos(thetap) + qperp * std::sin(thetap);
	}
}

float Dot(const Quaternion& first, const Quaternion& second)
{
	__m128 mul = _mm_mul_ps(first.m_Vector, second.m_Vector);
	__m128 shuffle = _mm_shuffle_ps(mul, mul, _MM_SHUFFLE(2, 3, 0, 1));
	__m128 sums = _mm_add_ps(mul, shuffle);
	shuffle = _mm_movehl_ps(shuffle, sums);
	sums = _mm_add_ss(sums, shuffle);
	return _mm_cvtss_f32(sums);
}
