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

class Quaternion
{
public:
	Quaternion() = default;
	Quaternion(float x, float y, float z, float w);
	Quaternion(const Quaternion& other) = default;

	Quaternion& operator=(const Quaternion& other);

	Quaternion operator+(const Quaternion& other) const;
	Quaternion& operator+=(const Quaternion& other);

	Quaternion operator-(const Quaternion& other) const;
	Quaternion& operator-=(const Quaternion& other);

	Quaternion operator*(float scalar) const;
	Quaternion& operator*=(float scalar);

	Quaternion operator/(float scalar) const;
	Quaternion& operator/=(float scalar);

	Quaternion& Normalize();
	Quaternion GetNormalized() const;

	float& X = reinterpret_cast<float*>(&m_Vector)[0];
	float& Y = reinterpret_cast<float*>(&m_Vector)[1];
	float& Z = reinterpret_cast<float*>(&m_Vector)[2];
	float& W = reinterpret_cast<float*>(&m_Vector)[3];

private:
	Quaternion(const __m128& vector);

	friend Quaternion operator*(float scalar, const Quaternion& quaternion);
	friend float Dot(const Quaternion& first, const Quaternion& second);

	__m128 m_Vector = _mm_set_ps(1.f, 0.f, 0.f, 0.f);
};

Quaternion operator*(float scalar, const Quaternion& quaternion);

// Both quaternions MUST BE normalized
Quaternion Slerp(const Quaternion& from, const Quaternion& to, float ratio);
float Dot(const Quaternion& first, const Quaternion& second);

template<>
struct fmt::formatter<Quaternion>
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
			"(X: {:" + ParseString + "}, Y: {:" + ParseString + "}, Z: {:" + ParseString + "}, W: {" + ParseString + "})";

		return end;
	}

	template<typename FormatContext>
	auto format(const Quaternion& quaternion, FormatContext& context)
	{
		return format_to(
			context.out(),
			ParseString,
			quaternion.X, quaternion.Y, quaternion.Z, quaternion.W
		);
	}
};
