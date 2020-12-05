#include "PCH.h"
#include "Renderers/Preview.h"

RENDERER(Preview, PreviewRenderer)

Color PreviewRenderer::TraceRay(const Scene& scene, const Ray& ray, MemoryArena& arena, uint16_t depth)
{
	return Color(0.5f, 0.f, 0.f);
}

