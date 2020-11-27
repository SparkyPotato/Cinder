#include "PCH.h"

#include "Matrix.h"

Matrix::Matrix()
{
	m_Columns[0] = _mm_set_ps(0.f, 0.f, 0.f, 1.f);
	m_Columns[1] = _mm_set_ps(0.f, 0.f, 1.f, 0.f);
	m_Columns[2] = _mm_set_ps(0.f, 1.f, 0.f, 0.f);
	m_Columns[3] = _mm_set_ps(1.f, 0.f, 0.f, 0.f);
}

Matrix::Matrix(float rows[4][4])
{
	m_Columns[0] = _mm_set_ps(rows[3][0], rows[2][0], rows[1][0], rows[0][0]);
	m_Columns[1] = _mm_set_ps(rows[3][1], rows[2][1], rows[1][1], rows[0][1]);
	m_Columns[2] = _mm_set_ps(rows[3][2], rows[2][2], rows[1][2], rows[0][2]);
	m_Columns[3] = _mm_set_ps(rows[3][3], rows[2][3], rows[1][3], rows[0][3]);
}

Matrix::Matrix(float e00, float e01, float e02, float e03,
	float e10, float e11, float e12, float e13,
	float e20, float e21, float e22, float e23,
	float e30, float e31, float e32, float e33)
{
	m_Columns[0] = _mm_set_ps(e30, e20, e10, e00);
	m_Columns[1] = _mm_set_ps(e31, e21, e11, e01);
	m_Columns[2] = _mm_set_ps(e32, e22, e12, e02);
	m_Columns[3] = _mm_set_ps(e33, e23, e13, e03);
}

Matrix::Matrix(__m128 columns[4])
{
	m_Columns[0] = columns[0];
	m_Columns[1] = columns[1];
	m_Columns[2] = columns[2];
	m_Columns[3] = columns[3];
}

Matrix Matrix::operator*(const Matrix& matrix) const
{
	__m128 rows[4];
	auto column0 = reinterpret_cast<const float*>(&m_Columns[0]);
	auto column1 = reinterpret_cast<const float*>(&m_Columns[1]);
	auto column2 = reinterpret_cast<const float*>(&m_Columns[2]);
	auto column3 = reinterpret_cast<const float*>(&m_Columns[3]);
	rows[0] = _mm_set_ps(column3[0], column2[0], column1[0], column0[0]);
	rows[1] = _mm_set_ps(column3[1], column2[1], column1[1], column0[1]);
	rows[2] = _mm_set_ps(column3[2], column2[2], column1[2], column0[2]);
	rows[3] = _mm_set_ps(column3[3], column2[3], column1[3], column0[3]);

	float product[4][4];
	for (int row = 0; row < 4; row++)
	{
		for (int column = 0; column < 4; column++)
		{
			// Dot product
			__m128 mul = _mm_mul_ps(rows[row], matrix.m_Columns[column]);
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


float& Matrix::GetValue(uint8_t row, uint8_t column)
{
	ASSERT(row < 4 && column < 4, "Matrix index out of range!");

	auto columnArray = reinterpret_cast<float*>(&m_Columns[column]);
	return columnArray[row];
}

float Matrix::GetValue(uint8_t row, uint8_t column) const
{
	ASSERT(row < 4 && column < 4, "Matrix index out of range!");
	
	auto columnArray = reinterpret_cast<const float*>(&m_Columns[column]);
	return columnArray[row];
}

Matrix Matrix::Transpose() const
{
	__m128 rows[4];
	auto column0 = reinterpret_cast<const float*>(&m_Columns[0]);
	auto column1 = reinterpret_cast<const float*>(&m_Columns[1]);
	auto column2 = reinterpret_cast<const float*>(&m_Columns[2]);
	auto column3 = reinterpret_cast<const float*>(&m_Columns[3]);
	rows[0] = _mm_set_ps(column3[0], column2[0], column1[0], column0[0]);
	rows[1] = _mm_set_ps(column3[1], column2[1], column1[1], column0[1]);
	rows[2] = _mm_set_ps(column3[2], column2[2], column1[2], column0[2]);
	rows[3] = _mm_set_ps(column3[3], column2[3], column1[3], column0[3]);

	return Matrix(rows);
}

// https://stackoverflow.com/a/44446912
Matrix Matrix::Inverse() const
{
	const float* columns[4];
	columns[0] = reinterpret_cast<const float*>(&m_Columns[0]);
	columns[1] = reinterpret_cast<const float*>(&m_Columns[1]);
	columns[2] = reinterpret_cast<const float*>(&m_Columns[2]);
	columns[3] = reinterpret_cast<const float*>(&m_Columns[3]);

	float A2323 = columns[2][2] * columns[3][3] - columns[2][3] * columns[3][2];
	float A1323 = columns[2][1] * columns[3][3] - columns[2][3] * columns[3][1];
	float A1223 = columns[2][1] * columns[3][2] - columns[2][2] * columns[3][1];
	float A0323 = columns[2][0] * columns[3][3] - columns[2][3] * columns[3][0];
	float A0223 = columns[2][0] * columns[3][2] - columns[2][2] * columns[3][0];
	float A0123 = columns[2][0] * columns[3][1] - columns[2][1] * columns[3][0];
	float A2313 = columns[1][2] * columns[3][3] - columns[1][3] * columns[3][2];
	float A1313 = columns[1][1] * columns[3][3] - columns[1][3] * columns[3][1];
	float A1213 = columns[1][1] * columns[3][2] - columns[1][2] * columns[3][1];
	float A2312 = columns[1][2] * columns[2][3] - columns[1][3] * columns[2][2];
	float A1312 = columns[1][1] * columns[2][3] - columns[1][3] * columns[2][1];
	float A1212 = columns[1][1] * columns[2][2] - columns[1][2] * columns[2][1];
	float A0313 = columns[1][0] * columns[3][3] - columns[1][3] * columns[3][0];
	float A0213 = columns[1][0] * columns[3][2] - columns[1][2] * columns[3][0];
	float A0312 = columns[1][0] * columns[2][3] - columns[1][3] * columns[2][0];
	float A0212 = columns[1][0] * columns[2][2] - columns[1][2] * columns[2][0];
	float A0113 = columns[1][0] * columns[3][1] - columns[1][1] * columns[3][0];
	float A0112 = columns[1][0] * columns[2][1] - columns[1][1] * columns[2][0];

	float det = columns[0][0] * (columns[1][1] * A2323 - columns[1][2] * A1323 + columns[1][3] * A1223)
		- columns[0][1] * (columns[1][0] * A2323 - columns[1][2] * A0323 + columns[1][3] * A0223)
		+ columns[0][2] * (columns[1][0] * A1323 - columns[1][1] * A0323 + columns[1][3] * A0123)
		- columns[0][3] * (columns[1][0] * A1223 - columns[1][1] * A0223 + columns[1][2] * A0123);
	det = 1 / det;

	float m00 = det * (columns[1][1] * A2323 - columns[1][2] * A1323 + columns[1][3] * A1223);
	float m01 = det * -(columns[0][1] * A2323 - columns[0][2] * A1323 + columns[0][3] * A1223);
	float m02 = det * (columns[0][1] * A2313 - columns[0][2] * A1313 + columns[0][3] * A1213);
	float m03 = det * -(columns[0][1] * A2312 - columns[0][2] * A1312 + columns[0][3] * A1212);
	float m10 = det * -(columns[1][0] * A2323 - columns[1][2] * A0323 + columns[1][3] * A0223);
	float m11 = det * (columns[0][0] * A2323 - columns[0][2] * A0323 + columns[0][3] * A0223);
	float m12 = det * -(columns[0][0] * A2313 - columns[0][2] * A0313 + columns[0][3] * A0213);
	float m13 = det * (columns[0][0] * A2312 - columns[0][2] * A0312 + columns[0][3] * A0212);
	float m20 = det * (columns[1][0] * A1323 - columns[1][1] * A0323 + columns[1][3] * A0123);
	float m21 = det * -(columns[0][0] * A1323 - columns[0][1] * A0323 + columns[0][3] * A0123);
	float m22 = det * (columns[0][0] * A1313 - columns[0][1] * A0313 + columns[0][3] * A0113);
	float m23 = det * -(columns[0][0] * A1312 - columns[0][1] * A0312 + columns[0][3] * A0112);
	float m30 = det * -(columns[1][0] * A1223 - columns[1][1] * A0223 + columns[1][2] * A0123);
	float m31 = det * (columns[0][0] * A1223 - columns[0][1] * A0223 + columns[0][2] * A0123);
	float m32 = det * -(columns[0][0] * A1213 - columns[0][1] * A0213 + columns[0][2] * A0113);
	float m33 = det * (columns[0][0] * A1212 - columns[0][1] * A0212 + columns[0][2] * A0112);

	return Matrix(
		m00, m10, m20, m30,
		m01, m11, m21, m31,
		m02, m12, m22, m32,
		m03, m13, m23, m33
	);
}

bool operator==(const Matrix& first, const Matrix& second)
{
	for (int column = 0; column < 4; column++)
	{
		auto column1 = reinterpret_cast<const float*>(&first.m_Columns[column]);
		auto column2 = reinterpret_cast<const float*>(&second.m_Columns[column]);
		for (int row = 0; row < 4; row++)
		{
			if (!IsNearlyEqual(column1[row], column2[row])) { return false; }
		}
	}

	return true;
}

bool operator!=(const Matrix& first, const Matrix& second)
{
	return !(first == second);
}
