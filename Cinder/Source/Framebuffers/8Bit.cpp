#include "PCH.h"
#include "Framebuffers/8Bit.h"

FRAMEBUFFER(8-bit, EightBitFramebuffer)

EightBitFramebuffer::EightBitFramebuffer(uint32_t width, uint32_t height)
	: Framebuffer(width, height)
{}

BufferTile* EightBitFramebuffer::GetBufferTile(uint32_t xmin, uint32_t xmax, uint32_t ymin, uint32_t ymax)
{
	return nullptr;
}

void EightBitFramebuffer::SaveBufferTile(BufferTile* tile)
{
	
}

bool EightBitFramebuffer::Ouput(const std::string& file)
{
	return true;
}

bool EightBitFramebuffer::Parse(const YAML::Node& node)
{
	return true;
}

Color EightBitTile::GetPixel(uint32_t x, uint32_t y)
{
	return Color();
}

void EightBitTile::SetPixel(const Color& color, uint32_t x, uint32_t y)
{
	
}
