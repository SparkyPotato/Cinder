#include "PCH.h"
#include "Renderers/Whitted.h"

#include "BxDFs/Lambertian.h"

RENDERER(Whitted, WhittedRenderer)

Color WhittedRenderer::TraceRay(const Scene& scene, const Ray& ray, MemoryArena& arena, uint16_t depth)
{
 	RayIntersection intersection;
	if (scene.Intersect(ray, intersection))
	{
		auto brdf = arena.Allocate<LambertianBRDF>(Color(1.f));

		auto view = (Point() - intersection.HitPoint).GetNormalized();

		return brdf->Evaluate(view, Vector(0.f, -1.f, 0.f)) * Dot(intersection.HitNormal, view);
	}

	return Color();
}

