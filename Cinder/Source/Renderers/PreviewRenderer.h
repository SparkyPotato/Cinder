#pragma once

#include "Core/Components/Renderer.h"

class PreviewRenderer : public Renderer 
{
public:
	PreviewRenderer();

	void Render(const Scene& scene, Framebuffer& framebuffer) override;
};
