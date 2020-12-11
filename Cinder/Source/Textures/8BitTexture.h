#pragma once

#include "Core/Material/Texture.h"

class EightBitTexture : public Texture
{
public:
	EightBitTexture() = default;
	EightBitTexture(Color* data, uint32_t width, uint32_t height);
	virtual ~EightBitTexture();

	virtual Color Evaluate(const Interaction& interaction) override;

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
