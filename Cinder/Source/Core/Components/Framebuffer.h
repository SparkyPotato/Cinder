#pragma once

#include "ComponentManager.h"
#include "Core/Math/Color.h"

class BufferTile
{
public:
	BufferTile(Color* data, uint32_t stride, uint32_t xmin, uint32_t xmax, uint32_t ymin, uint32_t ymax)
		: m_Data(data), m_Stride(stride), m_Xmin(xmin), m_Xmax(xmax), m_Ymin(ymin), m_Ymax(ymax)
	{}

	Color& GetPixel(uint32_t x, uint32_t y)
	{
		ASSERT(x >= m_Xmin && x < m_Xmax, "Buffer Tile out of range!");
		ASSERT(y >= m_Ymin && y < m_Ymax, "Buffer Tile out of range!");

		return m_Data[m_Stride * (y - m_Ymin) + x - m_Xmin];
	}

private:
	uint32_t m_Stride;
	uint32_t m_Xmin, m_Xmax, m_Ymin, m_Ymax;
	Color* m_Data;
};

class Framebuffer
{
public:
	Framebuffer(uint32_t width, uint32_t height)
		: Width(width), Height(height)
	{}

	virtual ~Framebuffer() {}

	uint32_t Width, Height;

	virtual bool ParseSettings(const YAML::Node& node) { return true; }

	virtual BufferTile GetBufferTile(uint32_t xmin, uint32_t xmax, uint32_t ymin, uint32_t ymax) = 0;
	virtual void SaveBufferTile(BufferTile tile) = 0;
	virtual const Color* GetOutputColorData() const = 0;
};

#define REGISTER_FRAMEBUFFER(name, className) \
Framebuffer* Spawn##className(uint32_t width, uint32_t height) { return new className(width, height); } \
struct Register##className \
{ \
	Register##className() \
	{ \
		ComponentManager::Get()->RegisterFramebuffer(#name, &Spawn##className); \
	} \
}; \
static Register##className StaticRegister##className;
