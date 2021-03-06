//    Copyright 2021 Shaye Garg
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

		if (bounces == 0 || specular)
		{
			if (!hit) 
			{
				for (auto& light : scene.GetLights())
				{
					out += b * light->EvaluateAlong(r);
				}
			}
			else if (i.HitObject->GetMaterial()->EmissionLight) 
			{ 
				out += i.HitObject->GetMaterial()->GetEmission()->Evaluate(i)
					* i.HitObject->GetMaterial()->GetEmissionIntensity();
			}
		}
		if (!hit || bounces >= m_Depth) { break; }

		i.Outgoing = -r.Direction;

		i.HitObject->GetMaterial()->Compute(i, arena);
		out += b * SampleOneLight(scene, i, arena, sampler);

		Vector incoming;
		float pdf;
		BxDF::Type type;

		Color c = i.Bsdf->Sample(i.Outgoing, incoming, sampler, pdf, BxDF::All, &type);
		specular = type & BxDF::Specular;

		if (c == Color() || pdf == 0.f) { break; }

		float dot = Dot(incoming, i.SNormal);
		b *= c * std::abs(dot) / pdf;
		r = Ray(i.HitPoint + (dot < 0.f ? -Vector(i.GNormal) : Vector(i.GNormal)) * Epsilon, incoming);

		// Russian Roulette
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

// Light sampling - seems to provide the best results for a wide variety of scenes.
Color PathRenderer::Estimate(const Scene& scene, const Interaction& i, Sampler* sampler, Light* light, MemoryArena& arena)
{
	Vector incoming;
	float pdf;
	Occlusion tester;
	Color c = light->Sample(i, sampler, incoming, pdf, tester);
	if (c != Color() && pdf != 0.f)
	{
		return tester(scene) ? Color() :
			c * std::abs(Dot(incoming, i.SNormal)) / pdf * i.Bsdf->Evaluate(i.Outgoing, incoming);
	}

	return Color();
}

Color PathRenderer::SampleOneLight(const Scene& scene, const Interaction& interaction, MemoryArena& arena, Sampler* sampler)
{
	uint64_t lightCount = scene.GetLights().size();
	if (lightCount == 0) { return Color(); }
	uint64_t n = std::min(uint64_t(sampler->Get1D() * lightCount), lightCount - 1);

	Light* light = scene.GetLights()[n].get();
	return Estimate(scene, interaction, sampler, light, arena) * float(lightCount);
}
