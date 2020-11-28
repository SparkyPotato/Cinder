#pragma once

#include "Core/Components/Renderer.h"

class PreviewRenderer : public Renderer 
{
public:
	void Render(const Scene& scene) override;
};
