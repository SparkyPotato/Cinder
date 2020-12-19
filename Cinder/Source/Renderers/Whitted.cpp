#include "PCH.h"
#include "Renderers/Whitted.h"

RENDERER(Whitted, WhittedRenderer)

Color WhittedRenderer::TraceRay(const Scene& scene, const Ray& ray, MemoryArena& arena, Sampler* sampler, uint16_t depth)
{
 	Interaction interaction;
	if (!scene.Intersect(ray, interaction))
	{
		return scene.GetEnvironment().Sample(ray.Direction);
	}

	if (interaction.HitObject->GetEmission()) { return interaction.HitObject->GetEmission()->Evaluate(interaction); }

	interaction.HitObject->GetMaterial()->Compute(interaction, arena);
	const BSDF* bsdf = interaction.Bsdf;
	Vector outgoing = -ray.Direction;

	Color out;

	// Environment Map IBL
	out += bsdf->Evaluate(outgoing, Vector(interaction.GNormal)) *
		scene.GetEnvironment().SampleIrradiance(Vector(interaction.GNormal));

	for (auto& emission : scene.GetEmission())
	{
		Color avg;
 		for (uint32_t i = 0; i < emission->SampleCount; i++)
 		{
 			Vector incoming;
 			float pdf;
 			Occlusion occlusion;
 
 			Color lightColor = emission->Sample(interaction, sampler, incoming, pdf, occlusion);
 
 			if (lightColor == Color() || pdf == 0.f) { continue; }
 
 			Color c = bsdf->Evaluate(outgoing, incoming);
 			if (c != Color() && !occlusion(scene))
 			{
 				avg += c * lightColor * std::abs(Dot(incoming, interaction.SNormal)) / pdf;
 			}
 		}
 		out += avg / emission->SampleCount;
	}
	
	// Reflect
	if (depth + 1 < m_Depth)
	{
		out += SpecularReflect(scene, interaction, arena, sampler, depth);
		out += SpecularTransmit(scene, interaction, arena, sampler, depth);
	}

	return out;
}

bool WhittedRenderer::Parse(const YAML::Node& node)
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
