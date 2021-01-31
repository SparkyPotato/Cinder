//    Copyright 2021 Shaye Garg
//
//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
//    
//    http://www.apache.org/licenses/LICENSE-2.0
//    
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.

#pragma once

#include "Renderers/SamplerRenderer.h"

/// Unbiased Monte-Carlo path tracer.
class PathRenderer : public SamplerRenderer
{
public:
	virtual Color TraceRay(const Scene& scene, const Ray& ray, MemoryArena& arena, Sampler* sampler, uint16_t depth = 0) override;

	virtual bool Parse(const YAML::Node& node) override;

	Color Estimate(const Scene& scene, const Interaction& i, Sampler* sampler, Light* light, MemoryArena& arena);
	Color SampleOneLight(const Scene& scene, const Interaction& i, MemoryArena& arena, Sampler* sampler);

private:
	uint16_t m_Depth;
};
