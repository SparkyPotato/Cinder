#include "PCH.h"
#include "Renderers/Path.h"

RENDERER(Path, PathRenderer)

Color PathRenderer::TraceRay(const Scene& scene, const Ray& ray, MemoryArena& arena, Sampler* sampler, uint16_t depth)
{
	Color out, b(1.f);
	bool specular = false;
	Ray r(ray);

	for (uint16_t bounces = 0; ; bounces++)
	{
		Interaction i;
		bool hit = scene.Intersect(r, i);

		if (depth == 0 || specular)
		{
			if (hit && i.HitObject->GetEmission()) { out += b * i.HitObject->GetEmission()->Evaluate(i); break; }
			else if (!hit) { out += b * scene.GetEnvironment().Sample(r.Direction); }
		}
		if (!hit || bounces >= m_Depth || i.HitObject->GetEmission()) { break; }

		i.HitObject->GetMaterial()->Compute(i, arena);
		out += b * SampleOneLight(scene, i, arena, sampler);

		Vector outgoing = -r.Direction.GetNormalized(), incoming;
		float pdf;
		BxDF::Type type;
		Color c = i.Bsdf->EvaluateSample(outgoing, incoming, sampler, pdf, BxDF::All, &type);

		if (c == Color() || pdf == 0.f) { break; }

		b *= c * std::abs(Dot(incoming, i.SNormal)) / pdf;
		specular = (type & BxDF::Specular) != 0;
		r = Ray(i.HitPoint + Vector(i.GNormal) * Epsilon, incoming);

		if (bounces > 3) 
		{
			float q = std::max(0.05f, 1.f - b.Y());
			if (sampler->Get1D() < q) { break; }
			b /= 1.f - q;
		}
	}

	return out;
}

bool PathRenderer::Parse(const YAML::Node& node)
{
	bool b = SamplerRenderer::Parse(node);
	if (!b) { return b; }

	if (!node["Ray Depth"])
	{
		Error("No ray depth present (line {})!", node.Mark().line + 1);
		return false;
	}
	try { m_Depth = node["Ray Depth"].as<uint16_t>(); }
	catch (YAML::Exception& e)
	{
		Error("Ray depth must be an unsigned integer (line {})!", e.mark.line + 1);
		return false;
	}

	return true;
}

Color Estimate(const Scene& scene, const Interaction& i, Sampler* sampler, Emission* light, MemoryArena& arena, bool specular = false)
{
	BxDF::Type flags = specular ? BxDF::All : BxDF::Type(BxDF::All & ~BxDF::Specular);
	Color L;

	Vector incoming;
	Vector outgoing = Point() - i.HitPoint;
	outgoing.Normalize();

	float lightPdf, bsdfPdf;
	Occlusion occlusion;
	Color c = light->Sample(i, sampler, incoming, lightPdf, occlusion);
	if (lightPdf > 0.f && c != Color())
	{
		Color f;
		f = i.Bsdf->Evaluate(outgoing, incoming, flags) * std::abs(Dot(incoming, i.SNormal));
		bsdfPdf = i.Bsdf->Pdf(outgoing, incoming, flags);

		if (!occlusion(scene))
		{
			float weight = PowerHeuristic(1, lightPdf, 1, bsdfPdf);
			L = f * c * weight / lightPdf;
		}
	}

	BxDF::Type sType;
	c = i.Bsdf->EvaluateSample(outgoing, incoming, sampler, bsdfPdf, flags, &sType);
	c *= std::abs(Dot(incoming, i.SNormal));

	if (c != Color() && bsdfPdf > 0.f)
	{
		Interaction interaction;
		bool hit = scene.Intersect(Ray(i.HitPoint + Vector(i.GNormal) * Epsilon, incoming), interaction);
		if (hit && interaction.HitObject->GetEmission())
		{
			lightPdf = 1.f / interaction.HitObject->GetGeometry()->GetArea();
			float weight = PowerHeuristic(1, lightPdf, 1, bsdfPdf);
			L += interaction.HitObject->GetEmission()->Evaluate(interaction) * weight / bsdfPdf;
		}
	}

	return L;
}

Color PathRenderer::SampleOneLight(const Scene& scene, const Interaction& interaction, MemoryArena& arena, Sampler* sampler)
{
	uint64_t lightCount = scene.GetEmission().size();
	if (lightCount == 0) { return Color(); }
	uint64_t n = uint64_t(sampler->Get1D() * lightCount);
	if (n == lightCount) { return scene.GetEnvironment().SampleIrradiance(Vector(interaction.SNormal)); }

	Emission* light = scene.GetEmission()[n];
	return Estimate(scene, interaction, sampler, light, arena) * float(lightCount);
}

