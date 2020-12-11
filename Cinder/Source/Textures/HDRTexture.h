#pragma once

#include "Core/Material/Texture.h"

class HDRTexture : public Texture
{
public:
	HDRTexture() = default;
	HDRTexture(Color* data, uint32_t width, uint32_t height);
	virtual ~HDRTexture();

	virtual Color Evaluate(const Interaction& interaction) override;

	virtual bool Parse(const YAML::Node& node) override;

private:
	Color GetPixel(uint32_t x, uint32_t y);

	Color* m_Data;
	uint32_t m_Width, m_Height;
};
