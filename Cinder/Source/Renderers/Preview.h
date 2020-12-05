#pragma once

#include "Renderers/SamplerRenderer.h"

class PreviewRenderer : public SamplerRenderer
{
public:
	virtual Color TraceRay(const Scene& scene, const Ray& ray, MemoryArena& arena, uint16_t depth = 0) override;
};
