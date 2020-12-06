#include "PCH.h"
#include "SamplerRenderer.h"

void SamplerRenderer::Render(const Scene& scene, Framebuffer& framebuffer)
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
		m_Threads.emplace_back(&SamplerRenderer::Thread, this);
	}

	for (auto& thread : m_Threads)
	{
		thread.join();
	}
}

bool SamplerRenderer::Parse(const YAML::Node& node)
{
	if (!node["Sampler"])
	{
		Error("No Sampler specified (line {})!", node.Mark().line + 1);
		return false;
	}
	try { m_Sampler = node["Sampler"].as<std::string>(); }
	catch (YAML::Exception& e)
	{
		Error("Sampler type must be a string (line {})!", e.mark.line + 1);
		return false;
	}
	try { auto sampler = Registry::Get()->GSamplers.at(m_Sampler)(); }
	catch (...)
	{
		Error("Sampler type '{}' does not exist (line {})!", m_Sampler, node["Sampler"].Mark().line + 1);
		return false;
	}

	if (!node["Samples"])
	{
		Warning("Using default sample count ({})", m_Samples);
	}
	else
	{
		try { m_Samples = node["Samples"].as<uint32_t>(); }
		catch (YAML::Exception& e)
		{
			Error("Sample count must be an unsigned integer (line {})!", e.mark.line + 1);
			return false;
		}
	}

	return true;
}

void SamplerRenderer::Thread()
{
	up<Sampler> sampler = Registry::Get()->GSamplers.at(m_Sampler)();

	unsigned int tile = std::atomic_fetch_add(&m_Tile, 1u);
	MemoryArena arena;
	while (tile < m_RenderTiles.size())
	{
		Tile& rTile = m_RenderTiles[tile];
		BufferTile* bTile = m_Framebuffer->GetBufferTile(rTile.Xmin, rTile.Xmax, rTile.Ymin, rTile.Ymax);

		for (uint32_t x = rTile.Xmin; x != rTile.Xmax; x++)
		{
			for (uint32_t y = rTile.Ymin; y != rTile.Ymax; y++)
			{
				Color color;

				for (uint32_t i = 0; i < m_Samples; i++)
				{
					float xval = float(sampler->GetSample(x + 0.5f, 0.5f)) / m_Framebuffer->Width;
					float yval = float(sampler->GetSample(y + 0.5f, 0.5f)) / m_Framebuffer->Height;
					auto ray = m_Scene->GetCamera().GetRay(xval, yval);

					color += TraceRay(*m_Scene, ray, arena);
				}
				color /= float(m_Samples);

				bTile->SetPixel(color, x, y);
				arena.Reset();
			}
		}

		m_Framebuffer->SaveBufferTile(bTile);

		tile = std::atomic_fetch_add(&m_Tile, 1u);
	}
}