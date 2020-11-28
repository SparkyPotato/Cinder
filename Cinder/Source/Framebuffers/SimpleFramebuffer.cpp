#include "PCH.h"
#include "SimpleFramebuffer.h"

REGISTER_FRAMEBUFFER(Simple, SimpleFramebuffer)

SimpleFramebuffer::SimpleFramebuffer(uint32_t width, uint32_t height)
	: Framebuffer(width, height)
{
	m_Data = new Color[width * height];
}

SimpleFramebuffer::~SimpleFramebuffer()
{
	delete[] m_Data;
}

const Color* SimpleFramebuffer::GetOutputColorData() const
{
	return m_Data;
}
