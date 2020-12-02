#pragma once

#include "Core/Math/Color.h"

class Texture
{
public:
	Texture() = default;
	Texture(uint32_t width, uint32_t height);

	const Color& GetPixel(uint32_t x, uint32_t y) const;

	uint32_t Width = 0, Height = 0;
	Color* Data;
};
