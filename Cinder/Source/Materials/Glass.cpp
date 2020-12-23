#include "PCH.h"
#include "Materials/Glass.h"

#include "BxDFs/Microfacet.h"
#include "BxDFs/Specular.h"

MATERIAL(Glass, Glass)

Glass::Glass(const std::string& name)
	: Material(name)
{}

void Glass::Compute(Interaction& interaction, MemoryArena& arena) const
{
    NormalMap(m_Normal, interaction);
	interaction.Bsdf = arena.Allocate<BSDF>(interaction);

	float eta = m_Eta->Evaluate(interaction).R();
	auto fresnel = arena.Allocate<FresnelDielectric>(1.f, eta);
	float roughness = TrowbridgeReitz::RoughnessToAlpha(m_Roughness->Evaluate(interaction).R());
	auto microfacet = arena.Allocate<TrowbridgeReitz>(roughness, roughness);
	Color c = m_Color->Evaluate(interaction);

	interaction.Bsdf->Add(arena.Allocate<MicrofacetReflection>(c, microfacet, fresnel));
	interaction.Bsdf->Add(arena.Allocate<MicrofacetTransmission>(c, microfacet, 1.f, eta));
}

bool Glass::Parse(const YAML::Node& node)
{
	if (!node["Color"])
	{
		Error("Glass material does not have color (line {})!", node.Mark().line + 1);
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
		Error("Glass material does not have roughness (line {})!", node.Mark().line + 1);
		return false;
	}
	try { m_Roughness = node["Roughness"].as<up<Texture>>(); }
	catch (...) { return false; }

	if (!node["Refractive Index"])
	{
		Error("Glass material does not have an index of refraction (line {})!", node.Mark().line + 1);
		return false;
	}
	try { m_Eta = node["Refractive Index"].as<up<Texture>>(); }
	catch (YAML::Exception& e) { return false; }

	return true;
}
