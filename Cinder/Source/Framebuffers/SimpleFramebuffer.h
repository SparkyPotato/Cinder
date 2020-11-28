#pragma once

#include "Core/Components/Framebuffer.h"

class SimpleFramebuffer : public Framebuffer
{
public:
	SimpleFramebuffer(uint32_t width, uint32_t height);
	virtual ~SimpleFramebuffer();

	const Color* GetOutputColorData() const override;

private:
	Color* m_Data;
};
