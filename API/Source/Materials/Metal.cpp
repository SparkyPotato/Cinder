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
#include "Materials/Metal.h"

#include "BxDFs/Microfacet.h"

MATERIAL(Metal, Metal)

Metal::Metal(const std::string& name)
	: Material(name)
{}

void Metal::Compute(Interaction& interaction, MemoryArena& arena) const
{
    NormalMap(m_Normal, interaction);
	interaction.Bsdf = arena.Allocate<BSDF>(interaction);

	auto fresnel = arena.Allocate<FresnelConductor>(1.f, m_Eta->Evaluate(interaction), m_K->Evaluate(interaction));
	float roughness = TrowbridgeReitz::RoughnessToAlpha(m_Roughness->Evaluate(interaction).R());
	auto microfacet = arena.Allocate<TrowbridgeReitz>(roughness, roughness);

	interaction.Bsdf->Add(arena.Allocate<MicrofacetReflection>(m_Color->Evaluate(interaction), microfacet, fresnel));
}

bool Metal::Parse(const YAML::Node& node)
{
	if (!node["Color"])
	{
		Error("Metal material does not have color (line {})!", node.Mark().line + 1);
		return false;
	}
	try { m_Color = node["Color"].as<up<Texture>>(); }
	catch (...) { return false; }

    if (!node["Normal"])
	{
		Error("Glass material does not have a normal map (line {})!", node.Mark().line + 1);
		return false;
	}
	try { m_Normal = node["Normal"].as<up<Texture>>(); }
	catch (...) { return false; }

	if (!node["Roughness"])
	{
		Error("Metal material does not have roughness (line {})!", node.Mark().line + 1);
		return false;
	}
	try { m_Roughness = node["Roughness"].as<up<Texture>>(); }
	catch (...) { return false; }

	if (!node["Refractive Index"])
	{
		Error("Metal material does not have refractive index (line {})!", node.Mark().line + 1);
		return false;
	}
	try { m_Eta = node["Refractive Index"].as<up<Texture>>(); }
	catch (...) { return false; }

	if (!node["Absorption"])
	{
		Error("Metal material does not have absorption (line {})!", node.Mark().line + 1);
		return false;
	}
	try { m_K = node["Absorption"].as<up<Texture>>(); }
	catch (...) { return false; }

	return true;
}
