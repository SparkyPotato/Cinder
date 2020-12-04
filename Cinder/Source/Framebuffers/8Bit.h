#pragma once

#include "Core/Components/Framebuffer.h"

class EightBitTile : public BufferTile
{
public:
	virtual Color GetPixel(uint32_t x, uint32_t y) override;
	virtual void SetPixel(const Color& color, uint32_t x, uint32_t y) override;
};

class EightBitFramebuffer : public Framebuffer
{
public:
	EightBitFramebuffer(uint32_t width, uint32_t height);

	virtual BufferTile* GetBufferTile(uint32_t xmin, uint32_t xmax, uint32_t ymin, uint32_t ymax) override;
	virtual void SaveBufferTile(BufferTile* tile) override;
	virtual bool Ouput(const std::string& file) override;

	virtual bool Parse(const YAML::Node& node) override;
};
