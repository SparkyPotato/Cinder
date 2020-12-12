#include "PCH.h"
#include "Materials/Metal.h"

#include "BxDFs/Microfacet.h"

MATERIAL(Metal, MetalMaterial)

MetalMaterial::MetalMaterial(const std::string& name)
	: Material(name)
{}

void MetalMaterial::Compute(Interaction& interaction, MemoryArena& arena) const
{
	interaction.Bsdf = arena.Allocate<BSDF>(interaction);

	float roughness = TrowbridgeReitz::RoughnessToAlpha(m_Roughness->Evaluate(interaction).R);

	auto microfacet = arena.Allocate<TrowbridgeReitz>(roughness, roughness);
	auto fresnel = arena.Allocate<FresnelConductor>(1.f, m_Eta->Evaluate(interaction), m_K->Evaluate(interaction));

	interaction.Bsdf->Add(arena.Allocate<MicrofacetReflection>(m_Color->Evaluate(interaction), microfacet, fresnel));
}

bool MetalMaterial::Parse(const YAML::Node& node)
{
	if (!node["Color"])
	{
		Error("Matte material does not have color (line {})!", node.Mark().line + 1);
		return false;
	}
	try { m_Color = node["Color"].as<up<Texture>>(); }
	catch (...) { return false; }

	if (!node["Roughness"])
	{
		Error("Matte material does not have roughness (line {})!", node.Mark().line + 1);
		return false;
	}
	try { m_Roughness = node["Roughness"].as<up<Texture>>(); }
	catch (...) { return false; }

	if (!node["Refractive Index"])
	{
		Error("Matte material does not have refractive index (line {})!", node.Mark().line + 1);
		return false;
	}
	try { m_Eta = node["Refractive Index"].as<up<Texture>>(); }
	catch (...) { return false; }

	if (!node["Absorption"])
	{
		Error("Matte material does not have absorption (line {})!", node.Mark().line + 1);
		return false;
	}
	try { m_K = node["Absorption"].as<up<Texture>>(); }
	catch (...) { return false; }

	return true;
}
