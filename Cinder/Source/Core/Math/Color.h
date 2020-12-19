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
