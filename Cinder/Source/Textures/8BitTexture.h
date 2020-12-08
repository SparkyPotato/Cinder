#pragma once

#include "Core/Material/Texture.h"

class EightBitTexture : public Texture
{
public:
	virtual ~EightBitTexture();

	virtual Color Evaluate(const RayIntersection& intersection) override;

	virtual bool Parse(const YAML::Node& node) override;

private:
	Color GetPixel(uint32_t x, uint32_t y);

	struct Pixel
	{
		uint8_t R, G, B;
	};

	Pixel* m_Data;
	uint32_t m_Width, m_Height;
};
