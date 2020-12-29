//    Copyright 2021 SparkyPotato
//
//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
//    
//    http://www.apache.org/licenses/LICENSE-2.0
//    
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.

#pragma once

// A 4x4 dimensional matrix
class Matrix
{
public:
	Matrix();
	Matrix(float rows[4][4]);
	Matrix(float e00, float e01, float e02, float e03,
		float e10, float e11, float e12, float e13,
		float e20, float e21, float e22, float e23,
		float e30, float e31, float e32, float e33);

	float* operator[](uint8_t column);
	const float* operator[](uint8_t column) const;
	
	Matrix operator*(const Matrix& matrix) const;
	Matrix& operator*=(const Matrix& matrix);

	float& GetValue(uint8_t row, uint8_t column);
	float GetValue(uint8_t row, uint8_t column) const;

	Matrix Transpose() const;
	Matrix Inverse() const;

private:
	Matrix(__m128 columns[4]);
	
	friend class Vector;
	friend class Point;
	friend class Normal;
	friend fmt::formatter<Matrix>;

	friend bool operator==(const Matrix& first, const Matrix& second);
	friend bool operator!=(const Matrix& first, const Matrix& second);
	
	// Stored column-major to speed-up
	// Vector-matrix multiplications
	__m128 m_Columns[4];
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
			"\r\n{" + ParseString + "}, {" + ParseString + "}, {" + ParseString + "}, {" + ParseString + "}"
			"\r\n{" + ParseString + "}, {" + ParseString + "}, {" + ParseString + "}, {" + ParseString + "}"
			"\r\n{" + ParseString + "}, {" + ParseString + "}, {" + ParseString + "}, {" + ParseString + "}"
			"\r\n{" + ParseString + "}, {" + ParseString + "}, {" + ParseString + "}, {" + ParseString + "}";

		return end;
	}

	template<typename FormatContext>
	auto format(const Matrix& matrix, FormatContext& context)
	{
		auto column0 = reinterpret_cast<const float*>(&matrix.m_Columns[0]);
		auto column1 = reinterpret_cast<const float*>(&matrix.m_Columns[1]);
		auto column2 = reinterpret_cast<const float*>(&matrix.m_Columns[2]);
		auto column3 = reinterpret_cast<const float*>(&matrix.m_Columns[3]);

		return format_to(
			context.out(),
			ParseString,
			column0[0], column1[0], column2[0], column3[0],
			column0[1], column1[1], column2[1], column3[1],
			column0[2], column1[2], column2[2], column3[2],
			column0[3], column1[3], column2[3], column3[3]
		);
	}
};
