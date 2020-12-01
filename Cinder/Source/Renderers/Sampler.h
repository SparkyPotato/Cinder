#pragma once

#include "Core/Components/Renderer.h"
#include "Core/Math/Color.h"
#include "Core/Components/AccelerationStructure.h"

class Sampler : public Renderer
{
public:
	Sampler() = default;

	void Render(const Scene& scene, Framebuffer& framebuffer) override;

	bool ParseSettings(const YAML::Node& node) override;

	virtual Color TraceRay(const Ray& ray) = 0;

protected:
	struct Tile
	{
		Tile(uint32_t xmin, uint32_t xmax, uint32_t ymin, uint32_t ymax)
			: Xmin(xmin), Xmax(xmax), Ymin(ymin), Ymax(ymax)
		{}
		uint32_t Xmin, Xmax;
		uint32_t Ymin, Ymax;
	};

	void Thread();

	uint32_t m_Supersamples = 1;

	std::vector<Tile> m_RenderTiles;
	std::vector<std::thread> m_Threads;
	std::atomic<unsigned int> m_Tile = 0;
	const Scene* m_Scene;
	Framebuffer* m_Framebuffer;
	uint32_t m_AdjustedWidth = 0;
	uint32_t m_AdjustedHeight = 0;
};
