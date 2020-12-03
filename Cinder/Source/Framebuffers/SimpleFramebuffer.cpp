#include "PCH.h"
#include "SimpleFramebuffer.h"

REGISTER_FRAMEBUFFER(Simple, SimpleFramebuffer)

SimpleFramebuffer::SimpleFramebuffer(uint32_t width, uint32_t height)
	: Framebuffer(width, height)
{
	m_Data = Memory::Get()->AllocateArr<Color>(width * height);
	Verbose("Simple Framebuffer created with resolution {}x{}.", width, height);
}

const Color* SimpleFramebuffer::GetOutputColorData() const
{
	return m_Data;
}

BufferTile SimpleFramebuffer::GetBufferTile(uint32_t xmin, uint32_t xmax, uint32_t ymin, uint32_t ymax)
{
	return BufferTile(m_Data + Width * ymin + xmin, Width, xmin, xmax, ymin, ymax);
}

void SimpleFramebuffer::SaveBufferTile(BufferTile tile)
{
	return;
}
