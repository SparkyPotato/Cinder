#pragma once

// Portable SIMD intrinsics
#include <immintrin.h>

class Matrix
{
public:
	Matrix();
	Matrix(float rows[4][4]);
	Matrix(float e00, float e01, float e02, float e03,
		float e10, float e11, float e12, float e13,
		float e20, float e21, float e22, float e23,
		float e30, float e31, float e32, float e33);

	Matrix operator*(const Matrix& matrix) const;
	Matrix& operator*=(const Matrix& matrix);

	float& GetValue(uint8_t row, uint8_t column);
	float GetValue(uint8_t row, uint8_t column) const;

	Matrix Transpose() const;
	Matrix Inverse() const;

private:
	Matrix(__m128 rows[4]);

	friend fmt::formatter<Matrix>;

	friend bool operator==(const Matrix& first, const Matrix& second);
	friend bool operator!=(const Matrix& first, const Matrix& second);

	__m128 m_Rows[4];
};

bool operator==(const Matrix& first, const Matrix& second);
bool operator!=(const Matrix& first, const Matrix& second);

template<>
struct fmt::formatter<Matrix>
{
	std::string ParseString;

	auto parse(format_parse_context& context)
	{
		auto end = std::find(context.begin(), context.end(), '}');

		if (end != context.end())
		{
			ParseString = std::string(context.begin(), end);
		}

		ParseString =
			"\n{" + ParseString + "}, {" + ParseString + "}, {" + ParseString + "}, {" + ParseString + "}"
			"\n{" + ParseString + "}, {" + ParseString + "}, {" + ParseString + "}, {" + ParseString + "}"
			"\n{" + ParseString + "}, {" + ParseString + "}, {" + ParseString + "}, {" + ParseString + "}"
			"\n{" + ParseString + "}, {" + ParseString + "}, {" + ParseString + "}, {" + ParseString + "}";

		return end;
	}

	template<typename FormatContext>
	auto format(const Matrix& matrix, FormatContext& context)
	{
		auto row0 = reinterpret_cast<const float*>(&matrix.m_Rows[0]);
		auto row1 = reinterpret_cast<const float*>(&matrix.m_Rows[1]);
		auto row2 = reinterpret_cast<const float*>(&matrix.m_Rows[2]);
		auto row3 = reinterpret_cast<const float*>(&matrix.m_Rows[3]);

		return format_to(
			context.out(),
			ParseString,
			row0[0], row0[1], row0[2], row0[3],
			row1[0], row1[1], row1[2], row1[3],
			row2[0], row2[1], row2[2], row2[3],
			row3[0], row3[1], row3[2], row3[3],
			);
	}
};
