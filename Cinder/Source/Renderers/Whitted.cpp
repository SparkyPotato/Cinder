#include "PCH.h"
#include "Renderers/Whitted.h"

RENDERER(Whitted, WhittedRenderer)

Color WhittedRenderer::TraceRay(const Scene& scene, const Ray& ray, MemoryArena& arena, uint16_t depth)
{
 	RayIntersection intersection;
	if (scene.Intersect(ray, intersection))
	{
		return { std::abs(intersection.HitNormal.X()), std::abs(intersection.HitNormal.Y()), std::abs(intersection.HitNormal.Z() )};
	}

	return Color();
}

