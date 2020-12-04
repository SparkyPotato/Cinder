#pragma once

#include "Core/Components/Renderer.h"
#include "Core/Math/Color.h"

class Sampler : public Renderer
{
public:
	Sampler() = default;

	virtual void Render(const Scene& scene, Framebuffer& framebuffer) override;

	virtual bool Parse(const YAML::Node& node) override;

	virtual Color TraceRay(const Scene& scene, const Ray& ray, MemoryArena& arena, uint16_t depth = 0) = 0;

private:
	struct Tile
	{
		Tile(uint32_t xmin, uint32_t xmax, uint32_t ymin, uint32_t ymax)
			: Xmin(xmin), Xmax(xmax), Ymin(ymin), Ymax(ymax)
		{}
		uint32_t Xmin, Xmax;
		uint32_t Ymin, Ymax;
	};

	void Thread();

	std::vector<Tile> m_RenderTiles;
	std::vector<std::thread> m_Threads;
	std::atomic<unsigned int> m_Tile = 0;
	const Scene* m_Scene = nullptr;
	Framebuffer* m_Framebuffer = nullptr;
};
