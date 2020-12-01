#include "PCH.h"
#include "Texture.h"

Texture::Texture(uint32_t width, uint32_t height)
	: Width(width), Height(height)
{}

const Color& Texture::GetPixel(uint32_t x, uint32_t y) const
{
	ASSERT(x < Width, "Sampling out of range!");
	ASSERT(y < Height, "Sampling out of range!");

	return Data[y * Height + x];
}
