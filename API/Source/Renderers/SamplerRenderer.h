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

#include "Core/Components/Renderer.h"
#include "Core/Components/Sampler.h"
#include "Core/Components/Filter.h"
#include "Core/Math/Color.h"

/// A base for Renderers that take multiple samples for every pixel.
class SamplerRenderer : public Renderer
{
public:
	SamplerRenderer() = default;

	virtual void Render(const Scene& scene, Framebuffer& framebuffer) override;

	virtual bool Parse(const YAML::Node& node) override;

protected:
	Color SpecularReflect(const Scene& scene, const Interaction& interaction, MemoryArena& arena, Sampler* sampler, uint16_t depth);
	Color SpecularTransmit(const Scene& scene, const Interaction& interaction, MemoryArena& arena, Sampler* sampler, uint16_t depth);
	
private:
	struct Tile
	{
		Tile(uint32_t xmin, uint32_t xmax, uint32_t ymin, uint32_t ymax)
			: Xmin(xmin), Xmax(xmax), Ymin(ymin), Ymax(ymax)
		{}
		uint32_t Xmin, Xmax;
		uint32_t Ymin, Ymax;
	};

	virtual Color TraceRay(const Scene& scene, const Ray& ray, MemoryArena& arena, Sampler* sampler, uint16_t depth = 0) = 0;
	
	void Thread();

	uint32_t m_Samples = 8;
	std::vector<Tile> m_RenderTiles;
	std::vector<std::thread> m_Threads;
	std::atomic<uint32_t> m_Tile = 0;

	const Scene* m_Scene = nullptr;
	Framebuffer* m_Framebuffer = nullptr;

	std::string m_Sampler;
	up<Filter> m_Filter;
};
