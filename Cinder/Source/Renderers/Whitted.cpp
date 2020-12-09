#include "PCH.h"
#include "Renderers/Whitted.h"

#include "BxDFs/Lambertian.h"
#include "Core/Material/BSDF.h"

RENDERER(Whitted, WhittedRenderer)

Color WhittedRenderer::TraceRay(const Scene& scene, const Ray& ray, MemoryArena& arena, Sampler* sampler, uint16_t depth)
{
 	RayIntersection intersection;
	if (scene.Intersect(ray, intersection))
	{
		BSDF bsdf(intersection, intersection.HitNormal);
		bsdf.Add(arena.Allocate<LambertianBRDF>(Color(1.f)), 1.f);
		
		Color sample = bsdf.Evaluate(Point() - intersection.HitPoint, Vector(intersection.HitNormal));
		sample *= scene.GetEnvironment().SampleIrradiance(Vector(intersection.HitNormal));
		
		return sample;
	}

	return scene.GetEnvironment().Sample(ray.Direction);
}

