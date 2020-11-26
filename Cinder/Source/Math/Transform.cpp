#include "PCH.h"

#include "Transform.h"

Matrix::Matrix()
{
	m_Rows[0] = _mm_set_ps(0.f, 0.f, 0.f, 1.f);
	m_Rows[1] = _mm_set_ps(0.f, 0.f, 1.f, 0.f);
	m_Rows[2] = _mm_set_ps(0.f, 1.f, 0.f, 0.f);
	m_Rows[3] = _mm_set_ps(1.f, 0.f, 0.f, 0.f);
}

Matrix::Matrix(float rows[4][4])
{
	m_Rows[0] = _mm_load_ps(rows[0]);
	m_Rows[1] = _mm_load_ps(rows[1]);
	m_Rows[2] = _mm_load_ps(rows[2]);
	m_Rows[3] = _mm_load_ps(rows[3]);
}

Matrix::Matrix(float e00, float e01, float e02, float e03, 
			   float e10, float e11, float e12, float e13, 
			   float e20, float e21, float e22, float e23, 
			   float e30, float e31, float e32, float e33)
{
	m_Rows[0] = _mm_set_ps(e03, e02, e01, e00);
	m_Rows[1] = _mm_set_ps(e13, e12, e11, e10);
	m_Rows[2] = _mm_set_ps(e23, e22, e21, e20);
	m_Rows[3] = _mm_set_ps(e33, e32, e31, e30);
}

Matrix::Matrix(__m128 rows[4])
{
	m_Rows[0] = rows[0];
	m_Rows[1] = rows[1];
	m_Rows[2] = rows[2];
	m_Rows[3] = rows[3];
}

Matrix Matrix::operator*(const Matrix& matrix) const
{
	__m128 columns[4];
	auto row0 = reinterpret_cast<const float*>(&m_Rows[0]);
	auto row1 = reinterpret_cast<const float*>(&m_Rows[1]);
	auto row2 = reinterpret_cast<const float*>(&m_Rows[2]);
	auto row3 = reinterpret_cast<const float*>(&m_Rows[3]);
	columns[0] = _mm_set_ps(row3[0], row2[0], row1[0], row0[0]);
	columns[1] = _mm_set_ps(row3[1], row2[1], row1[1], row0[1]);
	columns[2] = _mm_set_ps(row3[2], row2[2], row1[2], row0[2]);
	columns[3] = _mm_set_ps(row3[3], row2[3], row1[3], row0[3]);

	float product[4][4];
	for (int row = 0; row < 4; row++)
	{
		for (int column = 0; column < 4; column++)
		{
			// Dot product
			__m128 mul = _mm_mul_ps(m_Rows[row], columns[column]);
			__m128 shuffle = _mm_shuffle_ps(mul, mul, _MM_SHUFFLE(2, 3, 0, 1));
			__m128 sums = _mm_add_ps(mul, shuffle);
			shuffle = _mm_movehl_ps(shuffle, sums);
			sums = _mm_add_ss(sums, shuffle);
			product[row][column] = _mm_cvtss_f32(sums);
		}
	}

	return Matrix(product);
}

Matrix& Matrix::operator*=(const Matrix& matrix)
{
	*this = *this * matrix;

	return *this;
}

Matrix Matrix::Transpose() const
{
	__m128 columns[4];
	auto row0 = reinterpret_cast<const float*>(&m_Rows[0]);
	auto row1 = reinterpret_cast<const float*>(&m_Rows[1]);
	auto row2 = reinterpret_cast<const float*>(&m_Rows[2]);
	auto row3 = reinterpret_cast<const float*>(&m_Rows[3]);
	columns[0] = _mm_set_ps(row3[0], row2[0], row1[0], row0[0]);
	columns[1] = _mm_set_ps(row3[1], row2[1], row1[1], row0[1]);
	columns[2] = _mm_set_ps(row3[2], row2[2], row1[2], row0[2]);
	columns[3] = _mm_set_ps(row3[3], row2[3], row1[3], row0[3]);

	return Matrix(columns);
}

Matrix Matrix::Inverse() const
{
	const float* rows[4];
	rows[0] = reinterpret_cast<const float*>(&m_Rows[0]);
	rows[1] = reinterpret_cast<const float*>(&m_Rows[1]);
	rows[2] = reinterpret_cast<const float*>(&m_Rows[2]);
	rows[3] = reinterpret_cast<const float*>(&m_Rows[3]);

	float A2323 = rows[2][2] * rows[3][3] - rows[2][3] * rows[3][2];
	float A1323 = rows[2][1] * rows[3][3] - rows[2][3] * rows[3][1];
	float A1223 = rows[2][1] * rows[3][2] - rows[2][2] * rows[3][1];
	float A0323 = rows[2][0] * rows[3][3] - rows[2][3] * rows[3][0];
	float A0223 = rows[2][0] * rows[3][2] - rows[2][2] * rows[3][0];
	float A0123 = rows[2][0] * rows[3][1] - rows[2][1] * rows[3][0];
	float A2313 = rows[1][2] * rows[3][3] - rows[1][3] * rows[3][2];
	float A1313 = rows[1][1] * rows[3][3] - rows[1][3] * rows[3][1];
	float A1213 = rows[1][1] * rows[3][2] - rows[1][2] * rows[3][1];
	float A2312 = rows[1][2] * rows[2][3] - rows[1][3] * rows[2][2];
	float A1312 = rows[1][1] * rows[2][3] - rows[1][3] * rows[2][1];
	float A1212 = rows[1][1] * rows[2][2] - rows[1][2] * rows[2][1];
	float A0313 = rows[1][0] * rows[3][3] - rows[1][3] * rows[3][0];
	float A0213 = rows[1][0] * rows[3][2] - rows[1][2] * rows[3][0];
	float A0312 = rows[1][0] * rows[2][3] - rows[1][3] * rows[2][0];
	float A0212 = rows[1][0] * rows[2][2] - rows[1][2] * rows[2][0];
	float A0113 = rows[1][0] * rows[3][1] - rows[1][1] * rows[3][0];
	float A0112 = rows[1][0] * rows[2][1] - rows[1][1] * rows[2][0];

	float det = rows[0][0] * (rows[1][1] * A2323 - rows[1][2] * A1323 + rows[1][3] * A1223)
		- rows[0][1] * (rows[1][0] * A2323 - rows[1][2] * A0323 + rows[1][3] * A0223)
		+ rows[0][2] * (rows[1][0] * A1323 - rows[1][1] * A0323 + rows[1][3] * A0123)
		- rows[0][3] * (rows[1][0] * A1223 - rows[1][1] * A0223 + rows[1][2] * A0123);
	det = 1 / det;

	float m00 = det * (rows[1][1] * A2323 - rows[1][2] * A1323 + rows[1][3] * A1223);
	float m01 = det * -(rows[0][1] * A2323 - rows[0][2] * A1323 + rows[0][3] * A1223);
	float m02 = det * (rows[0][1] * A2313 - rows[0][2] * A1313 + rows[0][3] * A1213);
	float m03 = det * -(rows[0][1] * A2312 - rows[0][2] * A1312 + rows[0][3] * A1212);
	float m10 = det * -(rows[1][0] * A2323 - rows[1][2] * A0323 + rows[1][3] * A0223);
	float m11 = det * (rows[0][0] * A2323 - rows[0][2] * A0323 + rows[0][3] * A0223);
	float m12 = det * -(rows[0][0] * A2313 - rows[0][2] * A0313 + rows[0][3] * A0213);
	float m13 = det * (rows[0][0] * A2312 - rows[0][2] * A0312 + rows[0][3] * A0212);
	float m20 = det * (rows[1][0] * A1323 - rows[1][1] * A0323 + rows[1][3] * A0123);
	float m21 = det * -(rows[0][0] * A1323 - rows[0][1] * A0323 + rows[0][3] * A0123);
	float m22 = det * (rows[0][0] * A1313 - rows[0][1] * A0313 + rows[0][3] * A0113);
	float m23 = det * -(rows[0][0] * A1312 - rows[0][1] * A0312 + rows[0][3] * A0112);
	float m30 = det * -(rows[1][0] * A1223 - rows[1][1] * A0223 + rows[1][2] * A0123);
	float m31 = det * (rows[0][0] * A1223 - rows[0][1] * A0223 + rows[0][2] * A0123);
	float m32 = det * -(rows[0][0] * A1213 - rows[0][1] * A0213 + rows[0][2] * A0113);
	float m33 = det * (rows[0][0] * A1212 - rows[0][1] * A0212 + rows[0][2] * A0112);

	return Matrix(
		m00, m01, m02, m03,
		m10, m11, m12, m13,
		m20, m21, m22, m23,
		m30, m31, m32, m33
	);
}

bool operator==(const Matrix& first, const Matrix& second)
{
	for (int row = 0; row < 4; row++)
	{
		auto row1 = reinterpret_cast<const float*>(&first.m_Rows[row]);
		auto row2 = reinterpret_cast<const float*>(&second.m_Rows[row]);
		for (int column = 0; column < 4; column++)
		{
			if (row1[column] != row2[column]) { return false; }
		}
	}

	return true;
}

bool operator!=(const Matrix& first, const Matrix& second)
{
	return !(first == second);
}
