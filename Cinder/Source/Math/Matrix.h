#pragma once
#include "Vector.h"

class Matrix
{
public:
	Matrix();
	Matrix(const Vector& row1, const Vector& row2, const Vector& row3, const Vector& row4);
	Matrix(Vector rows[4]);
	Matrix(const Matrix& other);

	Matrix operator*(const Matrix& other) const;
	Matrix& operator*=(const Matrix& other);

	Matrix& SetColumns(Vector columns[4]);
	Matrix& SetColumns(const Vector& column1, const Vector& column2, const Vector& column3, const Vector& column4);

	static Matrix Columns(Vector columns[4]);
	static Matrix Columns(const Vector& column1, const Vector& column2, const Vector& column3, const Vector& column4);

	static Matrix Scale(float scaleFactor);
	static Matrix Scale(const Vector& scaling);

	static Matrix Rotate(const Vector& rotationDegrees);
	static Matrix Rotate(const Vector& axis, float angleDegrees);

	static Matrix Translate(const Vector& translation);

	static Matrix Orthographic(float width, float height, float farPlane, float nearPlane);
	static Matrix Perspective(float width, float height, float farPlane, float nearPlane);

	static Matrix Inverse(const Matrix& matrixToInvert);

	// All the columns of the matrix.
	Vector columns[4];
};
