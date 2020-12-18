#include "PCH.h"
#include "Renderers/PathRenderer.h"

RENDERER(Path, PathRenderer)

Color PathRenderer::TraceRay(const Scene& scene, const Ray& ray, MemoryArena& arena, Sampler* sampler, uint16_t depth)
{
	if (depth > m_Depth) { return Color(); }

	Color out;
	for (auto& light : scene.GetLights()) { out += light->EvaluateAlong(ray); }
	if (out != Color()) { return out; }

	Interaction interaction;
	if (!scene.Intersect(ray, interaction))
	{
		return scene.GetEnvironment().Sample(ray.Direction);
	}

	interaction.HitObject->GetMaterial()->Compute(interaction, arena);
	const BSDF* bsdf = interaction.Bsdf;
	Vector outgoing = (Point() - interaction.HitPoint).GetNormalized();

	for (auto& light : scene.GetLights())
	{
		out += light->EvaluateAlong(ray);
	}

	for (uint32_t i = 0; i < m_BSDFSamples; i++)
	{
		Vector incoming;
		float pdf;

		Color c = bsdf->EvaluateSample(outgoing, incoming, sampler->Get2D(), pdf);

		Point origin = interaction.HitPoint + Vector(interaction.GNormal) * Epsilon;
		out += c * TraceRay(scene, Ray(origin, incoming), arena, sampler, depth + 1) / pdf;
	}
	out /= m_BSDFSamples;
	
	// Reflect
	out += SpecularReflect(scene, interaction, arena, sampler, depth);
	out += SpecularTransmit(scene, interaction, arena, sampler, depth);

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

	if (!node["BSDF Samples"])
	{
		Error("No BSDF sample count present (line {})!", node.Mark().line + 1);
		return false;
	}
	try { m_BSDFSamples = node["BSDF Samples"].as<uint32_t>(); }
	catch (YAML::Exception& e)
	{
		Error("BSDF sample count must be an unsigned integer (line {})!", e.mark.line + 1);
		return false;
	}
	
	return true;
}
