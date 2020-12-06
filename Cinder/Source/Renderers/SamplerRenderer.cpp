#include "PCH.h"
#include "SamplerRenderer.h"

#include "Core/Global/ProgressBar.h"

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

	ProgressBar bar(0.f, float(m_RenderTiles.size()), float(m_RenderTiles.size()) / 100.f);
	while (m_Tile < m_RenderTiles.size())
	{
		bar.Update(float(m_Tile));

		using namespace std::literals;
		std::this_thread::sleep_for(100ms);
	}
	bar.End();

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
	try { auto sampler = Registry::Get()->GSamplers.at(m_Sampler)(0); }
	catch (...)
	{
		Error("Sampler type '{}' does not exist (line {})!", m_Sampler, node["Sampler"].Mark().line + 1);
		return false;
	}

	if (!node["Filter"])
	{
		Error("No Filter specified (line {})!", node.Mark().line + 1);
		return false;
	}
	std::string filter;
	try { filter = node["Filter"].as<std::string>(); }
	catch (YAML::Exception& e)
	{
		Error("Filter type must be a string (line {})!", e.mark.line + 1);
		return false;
	}
	try { m_Filter = Registry::Get()->GFilters.at(filter)(Vector(0.5f, 0.5f, 0.f)); }
	catch (...)
	{
		Error("Filter type '{}' does not exist (line {})!", filter, node["Filter"].Mark().line + 1);
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
	up<Sampler> sampler = Registry::Get()->GSamplers.at(m_Sampler)(m_Samples);

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
				sampler->StartPixel(x, y);
				do
				{
					auto pair = sampler->Get2D();

					float xval = (float(x) + pair.first) / m_Framebuffer->Width;
					float yval = (float(y) + pair.second) / m_Framebuffer->Height;
					Ray ray = m_Scene->GetCamera().GetRay(xval, yval);

					color += TraceRay(*m_Scene, ray, arena) * m_Filter->Evaluate(Point(pair.first - 0.5f, pair.second - 0.5f, 0.f));
				} while (sampler->NextSample());
				color /= float(m_Samples);

				bTile->SetPixel(color, x, y);
				arena.Reset();
			}
		}

		m_Framebuffer->SaveBufferTile(bTile);

		tile = std::atomic_fetch_add(&m_Tile, 1u);
	}
}