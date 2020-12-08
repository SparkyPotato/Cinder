#include "PCH.h"
#include "Renderers/Whitted.h"

#include "BxDFs/Lambertian.h"

RENDERER(Whitted, WhittedRenderer)

Color WhittedRenderer::TraceRay(const Scene& scene, const Ray& ray, MemoryArena& arena, Sampler* sampler, uint16_t depth)
{
 	RayIntersection intersection;
	if (scene.Intersect(ray, intersection))
	{
		auto brdf = arena.Allocate<LambertianBRDF>(Color(1.f));

		auto view = (Point() - intersection.HitPoint).GetNormalized();

		Vector s, t;
		GenerateCoordinateSystem(Vector(intersection.HitNormal), s, t);
		view = Vector(Dot(view, s), Dot(view, intersection.HitNormal), Dot(view, t));
		Vector light = Vector(Dot(Vector(0.f, 1.f, 0.f), s), Dot(Vector(0.f, 1.f, 0.f), intersection.HitNormal), Dot(Vector(0.f, 1.f, 0.f), t));

		Color sample = brdf->Evaluate(view, light);

		return sample * Dot(Vector(0.f, 1.f, 0.f), light);
	}

	return Color();
}

