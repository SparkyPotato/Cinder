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
#include "Materials/Plastic.h"

#include "BxDFs/OrenNayar.h"
#include "BxDFs/Microfacet.h"

MATERIAL(Plastic, Plastic)

Plastic::Plastic(const std::string& name)
	: Material(name)
{}

void Plastic::Compute(Interaction& interaction, MemoryArena& arena) const
{
    NormalMap(m_Normal, interaction);
	interaction.Bsdf = arena.Allocate<BSDF>(interaction);

	float rough = m_Roughness->Evaluate(interaction).R();
	float roughness = TrowbridgeReitz::RoughnessToAlpha(rough);
	auto microfacet = arena.Allocate<TrowbridgeReitz>(roughness, roughness);
	auto fresnel = arena.Allocate<FresnelDielectric>(1.f, m_Eta->Evaluate(interaction).R());

	interaction.Bsdf->Add(arena.Allocate<OrenNayar>(m_DiffuseColor->Evaluate(interaction), rough));
	interaction.Bsdf->Add(arena.Allocate<MicrofacetReflection>(m_SpecularColor->Evaluate(interaction), microfacet, fresnel));
}

bool Plastic::Parse(const YAML::Node& node)
{
	if (!node["Diffuse Color"])
	{
		Error("Plastic material does not have diffuse color (line {})!", node.Mark().line + 1);
		return false;
	}
	try { m_DiffuseColor = node["Diffuse Color"].as<up<Texture>>(); }
	catch (...) { return false; }

	if (!node["Specular Color"])
	{
		Error("Plastic material does not have specular color (line {})!", node.Mark().line + 1);
		return false;
	}
	try { m_SpecularColor = node["Specular Color"].as<up<Texture>>(); }
	catch (...) { return false; }

    if (!node["Normal"])
	{
		Error("Glass material does not have a normal map (line {})!", node.Mark().line + 1);
		return false;
	}
	try { m_Normal = node["Normal"].as<up<Texture>>(); }
	catch (...) { return false; }

	if (!node["Refractive Index"])
	{
		Error("Plastic material does not have refractive index (line {})!", node.Mark().line + 1);
		return false;
	}
	try { m_Eta = node["Refractive Index"].as<up<Texture>>(); }
	catch (...) { return false; }

	if (!node["Roughness"])
	{
		Error("Plastic material does not have roughness (line {})!", node.Mark().line + 1);
		return false;
	}
	try { m_Roughness = node["Roughness"].as<up<Texture>>(); }
	catch (...) { return false; }

	return true;

}
