#pragma once

#include "Renderers/SamplerRenderer.h"

class WhittedRenderer : public SamplerRenderer
{
public:
	virtual Color TraceRay(const Scene& scene, const Ray& ray, MemoryArena& arena, Sampler* sampler, uint16_t depth = 0) override;
};
