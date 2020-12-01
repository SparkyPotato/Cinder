#pragma once

// Color used in the framebuffer
class Color
{
public:
	Color() = default;
	Color(float r, float g, float b);
	
	Color& operator=(const Color& other);
	
	Color operator+(const Color& other);
	Color& operator+=(const Color& other);

	Color operator-(const Color& other);
	Color& operator-=(const Color& other);

	Color operator*(const Color& other);
	Color& operator*=(const Color& other);

	Color operator/(const Color& other);
	Color& operator/=(const Color& other);

	float& R = reinterpret_cast<float*>(&m_Vector)[0];
	float& G = reinterpret_cast<float*>(&m_Vector)[1];
	float& B = reinterpret_cast<float*>(&m_Vector)[2];

private:
	Color(__m128 vector);

	__m128 m_Vector = _mm_set_ps(0.f, 0.f, 0.f, 0.f);
};

template<>
struct YAML::convert<Color>
{
	static bool decode(const Node& node, Color& color);
};
