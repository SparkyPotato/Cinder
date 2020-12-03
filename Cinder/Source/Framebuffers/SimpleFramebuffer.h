#pragma once

#include "Core/Components/Framebuffer.h"

class SimpleFramebuffer : public Framebuffer
{
public:
	SimpleFramebuffer(uint32_t width, uint32_t height);

	const Color* GetOutputColorData() const override;


	BufferTile GetBufferTile(uint32_t xmin, uint32_t xmax, uint32_t ymin, uint32_t ymax) override;
	void SaveBufferTile(BufferTile tile) override;

private:
	Color* m_Data;
};
