#pragma once

#include "Renderers/Sampler.h"

class PreviewRenderer : public Sampler
{
public:
	virtual Color TraceRay(const Scene& scene, const Ray& ray, MemoryArena& arena, uint16_t depth = 0) override;
};
