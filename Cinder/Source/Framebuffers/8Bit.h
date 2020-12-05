#pragma once

#include "Core/Components/Framebuffer.h"

struct Pixel
{
	uint8_t R, G, B;
};

class EightBitTile : public BufferTile
{
public:
	EightBitTile(Pixel* data, uint32_t xmin, uint32_t xmax, uint32_t ymin, uint32_t ymax, uint32_t stride);
	
	virtual Color GetPixel(uint32_t x, uint32_t y) override;
	virtual void SetPixel(const Color& color, uint32_t x, uint32_t y) override;
	
private:
	Pixel* m_FramebufferData;
	uint32_t m_Stride;
	uint32_t m_Xmin, m_Xmax, m_Ymin, m_Ymax;
};

class EightBitFramebuffer : public Framebuffer
{
public:
	EightBitFramebuffer(uint32_t width, uint32_t height);
	virtual ~EightBitFramebuffer();

	virtual BufferTile* GetBufferTile(uint32_t xmin, uint32_t xmax, uint32_t ymin, uint32_t ymax) override;
	virtual void SaveBufferTile(BufferTile* tile) override;
	virtual bool Ouput(const std::string& file) override;

	virtual bool Parse(const YAML::Node& node) override;
	
private:
	Pixel* m_Data;
	MemoryArena m_Arena;
};
