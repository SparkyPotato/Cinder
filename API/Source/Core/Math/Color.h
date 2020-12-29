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

class Color
{
public:
	Color() = default;
	Color(float g);
	Color(float r, float g, float b);
	
	Color& operator=(const Color& other);
	
	Color operator+(const Color& other) const;
	Color& operator+=(const Color& other);

	Color operator-(const Color& other) const;
	Color& operator-=(const Color& other);

	Color operator*(const Color& other) const;
	Color& operator*=(const Color& other);

	Color operator/(const Color& other) const;
	Color& operator/=(const Color& other);

	float& R() { return reinterpret_cast<float*>(&m_Vector)[0]; }
	float& G() { return reinterpret_cast<float*>(&m_Vector)[1]; }
	float& B() { return reinterpret_cast<float*>(&m_Vector)[2]; }

	const float& R() const { return reinterpret_cast<const float*>(&m_Vector)[0]; }
	const float& G() const { return reinterpret_cast<const float*>(&m_Vector)[1]; }
	const float& B() const { return reinterpret_cast<const float*>(&m_Vector)[2]; }

	float Y() const;

private:
	Color(__m128 vector);

	__m128 m_Vector = _mm_set_ps(0.f, 0.f, 0.f, 0.f);
};

bool operator==(const Color& first, const Color& second);
bool operator!=(const Color& first, const Color& second);
bool operator<(const Color& first, const Color& second);
bool operator<=(const Color& first, const Color& second);
bool operator>(const Color& first, const Color& second);
bool operator>=(const Color& first, const Color& second);

Color Lerp(const Color& from, const Color& to, float ratio);

template<>
struct YAML::convert<Color>
{
	static bool decode(const Node& node, Color& color);
};
