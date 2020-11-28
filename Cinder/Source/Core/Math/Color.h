#pragma once

// Color used in the framebuffer
class Color
{
public:
	Color() = default;
	Color(float r, float g, float b);

	float R = 0.f, G = 0.f, B = 0.f;
};
