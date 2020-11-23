#include "Matrix.h"

#include "Math.h"

Matrix::Matrix()
{
	columns[0] = { 1.f, 0.f, 0.f, 0.f };
	columns[1] = { 0.f, 1.f, 0.f, 0.f };
	columns[2] = { 0.f, 0.f, 1.f, 0.f };
	columns[3] = { 0.f, 0.f, 0.f, 1.f };
}

Matrix::Matrix(const Vector& row1, const Vector& row2, const Vector& row3, const Vector& row4)
{
	columns[0].X = row1.X;
	columns[1].X = row1.Y;
	columns[2].X = row1.Z;
	columns[3].X = row1.W;

	columns[0].Y = row2.X;
	columns[1].Y = row2.Y;
	columns[2].Y = row2.Z;
	columns[3].Y = row2.W;

	columns[0].Z = row3.X;
	columns[1].Z = row3.Y;
	columns[2].Z = row3.Z;
	columns[3].Z = row3.W;

	columns[0].W = row4.X;
	columns[1].W = row4.Y;
	columns[2].W = row4.Z;
	columns[3].W = row4.W;
}

Matrix::Matrix(Vector rows[4])
{
	columns[0].X = rows[0].X;
	columns[1].X = rows[0].Y;
	columns[2].X = rows[0].Z;
	columns[3].X = rows[0].W;

	columns[0].Y = rows[1].X;
	columns[1].Y = rows[1].Y;
	columns[2].Y = rows[1].Z;
	columns[3].Y = rows[1].W;

	columns[0].Z = rows[2].X;
	columns[1].Z = rows[2].Y;
	columns[2].Z = rows[2].Z;
	columns[3].Z = rows[2].W;

	columns[0].W = rows[3].X;
	columns[1].W = rows[3].Y;
	columns[2].W = rows[3].Z;
	columns[3].W = rows[3].W;
}

Matrix::Matrix(const Matrix& other)
{
	columns[0] = other.columns[0];
	columns[1] = other.columns[1];
	columns[2] = other.columns[2];
	columns[3] = other.columns[3];
}

Matrix Matrix::operator*(const Matrix& other) const
{
	Matrix mat;

	for (int i = 0; i < 4; i++)
	{
		Vector row(columns[0].X, columns[1].X, columns[2].X, columns[3].X);
		mat.columns[i].X = Vector::Dot4D(row, other.columns[i]);

		row = { columns[0].Y, columns[1].Y, columns[2].Y, columns[3].Y };
		mat.columns[i].Y = Vector::Dot4D(row, other.columns[i]);

		row = { columns[0].Z, columns[1].Z, columns[2].Z, columns[3].Z };
		mat.columns[i].Z = Vector::Dot4D(row, other.columns[i]);

		row = { columns[0].W, columns[1].W, columns[2].W, columns[3].W };
		mat.columns[i].W = Vector::Dot4D(row, other.columns[i]);
	}

	return mat;
}

Matrix& Matrix::operator*=(const Matrix& other)
{
	*this = *this * other;

	return *this;
}

Matrix& Matrix::SetColumns(Vector newColumns[4])
{
	columns[0] = newColumns[0];
	columns[1] = newColumns[1];
	columns[2] = newColumns[2];
	columns[3] = newColumns[3];
	return *this;
}

Matrix& Matrix::SetColumns(const Vector& column1, const Vector& column2, const Vector& column3, const Vector& column4)
{
	columns[0] = column1;
	columns[1] = column2;
	columns[2] = column3;
	columns[3] = column4;
	return *this;
}

Matrix Matrix::Columns(Vector columns[4])
{
	Matrix mat;
	mat.columns[0] = columns[0];
	mat.columns[1] = columns[1];
	mat.columns[2] = columns[2];
	mat.columns[3] = columns[3];
	return mat;
}

Matrix Matrix::Columns(const Vector& column1, const Vector& column2, const Vector& column3, const Vector& column4)
{
	Matrix mat;
	mat.columns[0] = column1;
	mat.columns[1] = column2;
	mat.columns[2] = column3;
	mat.columns[3] = column4;
	return mat;
}

Matrix Matrix::Scale(float scaleFactor)
{
	Matrix mat =
	{
		{ scaleFactor, 0.f,         0.f,         0.f },
		{ 0.f,         scaleFactor, 0.f,         0.f },
		{ 0.f,         0.f,         scaleFactor, 0.f },
		{ 0.f,         0.f,         0.f,         1.f }
	};

	return mat;
}

Matrix Matrix::Scale(const Vector& scaling)
{
	Matrix mat =
	{
		{ scaling.X, 0.f,       0.f,       0.f },
		{ 0.f,       scaling.Y, 0.f,       0.f },
		{ 0.f,       0.f,       scaling.Z, 0.f },
		{ 0.f,       0.f,       0.f,       1.f }
	};

	return mat;
}

Matrix Matrix::Rotate(const Vector& rotation)
{
	auto sinx = (float)std::sin(-rotation.X * PI / 180.f);
	auto cosx = (float)std::cos(-rotation.X * PI / 180.f);
	auto siny = (float)std::sin(-rotation.Y * PI / 180.f);
	auto cosy = (float)std::cos(-rotation.Y * PI / 180.f);
	auto sinz = (float)std::sin(-rotation.Z * PI / 180.f);
	auto cosz = (float)std::cos(-rotation.Z * PI / 180.f);

	Matrix rotX =
	{
		{ 1.f, 0.f,   0.f,  0.f },
		{ 0.f, cosx,  sinx, 0.f },
		{ 0.f, -sinx, cosx, 0.f },
		{ 0.f, 0.f,   0.f,  1.f }
	};
	Matrix rotY =
	{
		{ cosy, 0.f, -siny, 0.f },
		{ 0.f,  1.f, 0.f,   0.f },
		{ siny, 0.f, cosy,  0.f },
		{ 0.f,  0.f, 0.f,   1.f }
	};
	Matrix rotZ =
	{
		{ cosz,  sinz, 0.f, 0.f },
		{ -sinz, cosz, 0.f, 0.f },
		{ 0.f,   0.f,  1.f, 0.f },
		{ 0.f,   0.f,  0.f, 1.f }
	};

	return rotX * rotY * rotZ;
}

Matrix Matrix::Rotate(const Vector& axis, float angleDegrees)
{
	Vector vec = axis.GetNormalized();
	float x = vec.X;
	float y = vec.Y;
	float z = vec.Z;
	auto cos = (float)std::cos(-angleDegrees * PI / 180.f);
	auto sin = (float)std::sin(-angleDegrees * PI / 180.f);

	Matrix mat =
	{
		{ cos + (x * x) * (1 - cos),    (x * y) * (1 - cos) + z * sin,  (x * z) * (1 - cos) - y * sin, 0.f },
		{ (x * y) * (1 - cos) - z * sin, cos + (y * y) * (1 - cos),     (y * z) * (1 - cos) + x * sin, 0.f },
		{ (x * z) * (1 - cos) + y * sin, (y * z) * (1 - cos) - x * sin, cos + (z * z) * (1 - cos),     0.f },
		{ 0.f,                           0.f,                           0.f,                           1.f }
	};

	return mat;
}

Matrix Matrix::Translate(const Vector& translation)
{
	Matrix mat =
	{
		{ 1.f,           0.f,           0.f,           0.f },
		{ 0.f,           1.f,           0.f,           0.f },
		{ 0.f,           0.f,           1.f,           0.f },
		{ translation.X, translation.Y, translation.Z, 1.f }
	};

	return mat;
}

Matrix Matrix::Orthographic(float width, float height, float farp, float nearp)
{
	Matrix mat =
	{
		{ 2.f / width, 0.f,          0.f,                     0.f },
		{ 0.f,         2.f / height, 0.f,                     0.f },
		{ 0.f,         0.f,          1.f / (farp - nearp),    0.f },
		{ 0.f,         0.f,          -nearp / (farp - nearp), 1.f }
	};

	return mat;
}

Matrix Matrix::Perspective(float width, float height, float farp, float nearp)
{
	Matrix mat =
	{
		{ 2.f * nearp / width, 0.f,                  0.f,                              0.f },
		{ 0.f,                 2.f * nearp / height, 0.f,                              0.f },
		{ 0.f,                 0.f,                  farp / (farp - nearp),            1.f },
		{ 0.f,                 0.f,                  -(farp * nearp) / (farp - nearp), 0.f }
	};

	return mat;
}

Matrix Matrix::Inverse(const Matrix& m)
{
	float A2323 = m.columns[2].Z * m.columns[3].W - m.columns[2].W * m.columns[3].Z;
	float A1323 = m.columns[2].Y * m.columns[3].W - m.columns[2].W * m.columns[3].Y;
	float A1223 = m.columns[2].Y * m.columns[3].Z - m.columns[2].Z * m.columns[3].Y;
	float A0323 = m.columns[2].X * m.columns[3].W - m.columns[2].W * m.columns[3].X;
	float A0223 = m.columns[2].X * m.columns[3].Z - m.columns[2].Z * m.columns[3].X;
	float A0123 = m.columns[2].X * m.columns[3].Y - m.columns[2].Y * m.columns[3].X;
	float A2313 = m.columns[1].Z * m.columns[3].W - m.columns[1].W * m.columns[3].Z;
	float A1313 = m.columns[1].Y * m.columns[3].W - m.columns[1].W * m.columns[3].Y;
	float A1213 = m.columns[1].Y * m.columns[3].Z - m.columns[1].Z * m.columns[3].Y;
	float A2312 = m.columns[1].Z * m.columns[2].W - m.columns[1].W * m.columns[2].Z;
	float A1312 = m.columns[1].Y * m.columns[2].W - m.columns[1].W * m.columns[2].Y;
	float A1212 = m.columns[1].Y * m.columns[2].Z - m.columns[1].Z * m.columns[2].Y;
	float A0313 = m.columns[1].X * m.columns[3].W - m.columns[1].W * m.columns[3].X;
	float A0213 = m.columns[1].X * m.columns[3].Z - m.columns[1].Z * m.columns[3].X;
	float A0312 = m.columns[1].X * m.columns[2].W - m.columns[1].W * m.columns[2].X;
	float A0212 = m.columns[1].X * m.columns[2].Z - m.columns[1].Z * m.columns[2].X;
	float A0113 = m.columns[1].X * m.columns[3].Y - m.columns[1].Y * m.columns[3].X;
	float A0112 = m.columns[1].X * m.columns[2].Y - m.columns[1].Y * m.columns[2].X;

	float det = m.columns[0].X * (m.columns[1].Y * A2323 - m.columns[1].Z * A1323 + m.columns[1].W * A1223)
		- m.columns[0].Y * (m.columns[1].X * A2323 - m.columns[1].Z * A0323 + m.columns[1].W * A0223)
		+ m.columns[0].Z * (m.columns[1].X * A1323 - m.columns[1].Y * A0323 + m.columns[1].W * A0123)
		- m.columns[0].W * (m.columns[1].X * A1223 - m.columns[1].Y * A0223 + m.columns[1].Z * A0123);
	det = 1.f / det;

	Matrix mat;

	mat.columns[0].X = det * (m.columns[1].Y * A2323 - m.columns[1].Z * A1323 + m.columns[1].W * A1223);
	mat.columns[0].Y = det * -(m.columns[0].Y * A2323 - m.columns[0].Z * A1323 + m.columns[0].W * A1223);
	mat.columns[0].Z = det * (m.columns[0].Y * A2313 - m.columns[0].Z * A1313 + m.columns[0].W * A1213);
	mat.columns[0].W = det * -(m.columns[0].Y * A2312 - m.columns[0].Z * A1312 + m.columns[0].W * A1212);
	mat.columns[1].X = det * -(m.columns[1].X * A2323 - m.columns[1].Z * A0323 + m.columns[1].W * A0223);
	mat.columns[1].Y = det * (m.columns[0].X * A2323 - m.columns[0].Z * A0323 + m.columns[0].W * A0223);
	mat.columns[1].Z = det * -(m.columns[0].X * A2313 - m.columns[0].Z * A0313 + m.columns[0].W * A0213);
	mat.columns[1].W = det * (m.columns[0].X * A2312 - m.columns[0].Z * A0312 + m.columns[0].W * A0212);
	mat.columns[2].X = det * (m.columns[1].X * A1323 - m.columns[1].Y * A0323 + m.columns[1].W * A0123);
	mat.columns[2].Y = det * -(m.columns[0].X * A1323 - m.columns[0].Y * A0323 + m.columns[0].W * A0123);
	mat.columns[2].Z = det * (m.columns[0].X * A1313 - m.columns[0].Y * A0313 + m.columns[0].W * A0113);
	mat.columns[2].W = det * -(m.columns[0].X * A1312 - m.columns[0].Y * A0312 + m.columns[0].W * A0112);
	mat.columns[3].X = det * -(m.columns[1].X * A1223 - m.columns[1].Y * A0223 + m.columns[1].Z * A0123);
	mat.columns[3].Y = det * (m.columns[0].X * A1223 - m.columns[0].Y * A0223 + m.columns[0].Z * A0123);
	mat.columns[3].Z = det * -(m.columns[0].X * A1213 - m.columns[0].Y * A0213 + m.columns[0].Z * A0113);
	mat.columns[3].W = det * (m.columns[0].X * A1212 - m.columns[0].Y * A0212 + m.columns[0].Z * A0112);

	return mat;
}
 