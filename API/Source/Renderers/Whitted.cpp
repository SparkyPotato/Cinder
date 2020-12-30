//    Copyright 2021 SparkyPotato
//
//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
//    
//    http://www.apache.org/licenses/LICENSE-2.0
//    
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.

#include "PCH.h"
#include "Renderers/Whitted.h"

RENDERER(Whitted, WhittedRenderer)

Color WhittedRenderer::TraceRay(const Scene& scene, const Ray& ray, MemoryArena& arena, Sampler* sampler, uint16_t depth)
{
 	Interaction interaction;
	Color out;

	if (!scene.Intersect(ray, interaction))
	{
		return scene.GetEnvironment().Sample(ray.Direction);
	}

	interaction.HitObject->GetMaterial()->Compute(interaction, arena);
	if (interaction.HitObject->GetMaterial()->GetEmission()) { out += interaction.HitObject->GetMaterial()->GetEmission()->Evaluate(interaction); }
	const BSDF* bsdf = interaction.Bsdf;
	Vector outgoing = -ray.Direction;

	// Environment Map IBL
	out += bsdf->Evaluate(outgoing, Vector(interaction.GNormal)) *
		scene.GetEnvironment().SampleIrradiance(Vector(interaction.GNormal));

	for (auto& light : scene.GetLights())
	{
		Color avg;
 		for (uint32_t i = 0; i < light->SampleCount; i++)
 		{
 			Vector incoming;
 			float pdf;
 			Occlusion occlusion;
 
 			Color lightColor = light->Sample(interaction, sampler, incoming, pdf, occlusion);
 
 			if (lightColor == Color() || pdf == 0.f) { continue; }
 
 			Color c = bsdf->Evaluate(outgoing, incoming);
 			if (c != Color() && !occlusion(scene))
 			{
 				avg += c * lightColor * std::abs(Dot(incoming, interaction.SNormal)) / pdf;
 			}
 		}
		out += avg / float(light->SampleCount);
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
