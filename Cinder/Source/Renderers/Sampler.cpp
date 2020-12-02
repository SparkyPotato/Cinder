#include "PCH.h"
#include "Sampler.h"

#include "Core/Components/Framebuffer.h"

void Sampler::Render(const Scene& scene, Framebuffer& framebuffer)
{
	m_Scene = &scene;
	m_Framebuffer = &framebuffer;

	for (uint32_t x = 0; x < framebuffer.Width; x += GOptions.TileSize)
	{
		uint32_t xMax = x + GOptions.TileSize;
		if (xMax > framebuffer.Width) { xMax = framebuffer.Width; }

		for (uint32_t y = 0; y < framebuffer.Height; y += GOptions.TileSize)
		{
			uint32_t yMax = y + GOptions.TileSize;
			if (yMax > framebuffer.Height) { yMax = framebuffer.Height; }

			m_RenderTiles.emplace_back(x, xMax, y, yMax);
		}
	}

	for (uint16_t i = 0; i < GOptions.ThreadCount; i++)
	{
		m_Threads.emplace_back(&Sampler::Thread, this);
	}

	for (auto& thread : m_Threads)
	{
		thread.join();
	}
}

bool Sampler::ParseSettings(const YAML::Node& node)
{
	return true;
}

void Sampler::Thread()
{
	unsigned int tile = std::atomic_fetch_add(&m_Tile, 1u);
	while (tile < m_RenderTiles.size())
	{
		Tile& rTile = m_RenderTiles[tile];
		BufferTile bTile = m_Framebuffer->GetBufferTile(rTile.Xmin, rTile.Xmax, rTile.Ymin, rTile.Ymax);

		for (uint32_t x = rTile.Xmin; x != rTile.Xmax; x++)
		{
			for (uint32_t y = rTile.Ymin; y != rTile.Ymax; y++)
			{
				float xval = float(x + 0.5f) / m_Framebuffer->Width;
				float yval = float(y + 0.5f) / m_Framebuffer->Height;

				auto ray = m_Scene->MainCamera->GetRay(xval, yval);
				bTile.GetPixel(x, y) = TraceRay(ray);
			}
		}

		m_Framebuffer->SaveBufferTile(bTile);

		tile = std::atomic_fetch_add(&m_Tile, 1u);
	}
}
