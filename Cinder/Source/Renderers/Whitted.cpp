#include "PCH.h"
#include "Renderers/Whitted.h"

RENDERER(Whitted, WhittedRenderer)

Color WhittedRenderer::TraceRay(const Scene& scene, const Ray& ray, MemoryArena& arena, Sampler* sampler, uint16_t depth)
{
 	RayIntersection intersection;
	if (!scene.Intersect(ray, intersection))
	{
		return scene.GetEnvironment().Sample(ray.Direction);
	}

	BSDF* bsdf = intersection.HitObject->GetMaterial()->GetBSDF(intersection, arena);
	Vector outgoing = (Point() - intersection.HitPoint).GetNormalized();

	Color out;

	// Environment Map IBL
	out += bsdf->Evaluate(outgoing, Vector(intersection.HitNormal)) *
		scene.GetEnvironment().SampleIrradiance(Vector(intersection.HitNormal));

	// Lights
	for (auto& light : scene.GetLights())
	{
		Vector incoming;
		float pdf;
		Occlusion occlusion;

		Color lightColor = light->EvaluateSample(intersection, sampler->Get2D(), incoming, pdf, occlusion);

		if (lightColor == Color() || pdf == 0.f) { continue; }

		Color c = bsdf->Evaluate(outgoing, incoming);
		if (c != Color() && !occlusion(scene))
		{
			out += c * lightColor * std::abs(Dot(incoming, intersection.HitNormal)) / pdf;
		}
	}

	return out;
}

