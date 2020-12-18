#pragma once

#include "Renderers/SamplerRenderer.h"

class PathRenderer : public SamplerRenderer
{
public:
    virtual Color TraceRay(const Scene& scene, const Ray& ray, MemoryArena& arena, Sampler* sampler, uint16_t depth = 0) override;
	
	virtual bool Parse(const YAML::Node& node) override;

private:
    uint16_t m_Depth;
    uint32_t m_BSDFSamples;
};
