#include "PCH.h"
#include "Renderers/Whitted.h"

RENDERER(Whitted, WhittedRenderer)

Color WhittedRenderer::TraceRay(const Scene& scene, const Ray& ray, MemoryArena& arena, Sampler* sampler, uint16_t depth)
{
 	RayIntersection intersection;
	if (scene.Intersect(ray, intersection))
	{
		BSDF* bsdf = intersection.HitObject->GetMaterial()->GetBSDF(intersection, arena);
		Vector outgoing = (Point() - intersection.HitPoint).GetNormalized();
		
		Color out;

		// Environment Map IBL
		out += bsdf->Evaluate(outgoing, Vector(intersection.HitNormal)) * 
			scene.GetEnvironment().SampleIrradiance(Vector(intersection.HitNormal));
		
		return out;
	}

	return scene.GetEnvironment().Sample(ray.Direction);
}

