#include "PCH.h"
#include "Renderers/Whitted.h"

#include "BxDFs/Lambertian.h"

RENDERER(Whitted, WhittedRenderer)

Color WhittedRenderer::TraceRay(const Scene& scene, const Ray& ray, MemoryArena& arena, Sampler* sampler, uint16_t depth)
{
 	RayIntersection intersection;
	if (scene.Intersect(ray, intersection))
	{
		Vector s, t;
		GenerateCoordinateSystem(Vector(intersection.HitNormal), s, t);
		
		auto brdf = arena.Allocate<LambertianBRDF>(Color(1.f, 0.f, 0.f));

		auto view = (Point() - intersection.HitPoint).GetNormalized();
		view = view.TransformTo(s, Vector(intersection.HitNormal), t);
		Vector light = Vector(0.f, 1.f, 0.f).TransformTo(s, Vector(intersection.HitNormal), t);

		Color sample = brdf->Evaluate(view, light);
		return sample * Dot(Vector(0.f, 1.f, 0.f), light);
	}

	return scene.GetEnvironment().Sample(ray.Direction);
}

