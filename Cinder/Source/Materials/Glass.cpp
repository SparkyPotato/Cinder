#include "PCH.h"
#include "Materials/Glass.h"

#include "BxDFs/Specular.h"

MATERIAL(Glass, Glass)

Glass::Glass(const std::string& name)
	: Material(name)
{}

void Glass::Compute(Interaction& interaction, MemoryArena& arena) const
{
	Vector gX, gZ;
	GenerateCoordinateSystem(Vector(interaction.GNormal), gX, gZ);
	Color n = m_Normal->Evaluate(interaction);
	interaction.SNormal = Normal(Vector(n.R, n.B, n.G).TransformFrom(gX, Vector(interaction.GNormal), gZ));

	interaction.Bsdf = arena.Allocate<BSDF>(interaction);
	auto fresnel = arena.Allocate<FresnelDielectric>(1.f, m_Eta);
	Color c = m_Color->Evaluate(interaction);
	interaction.Bsdf->Add(arena.Allocate<SpecularReflection>(c, fresnel), 1.f);
	interaction.Bsdf->Add(arena.Allocate<SpecularTransmission>(c, 1.f, m_Eta), 1.f);
}

bool Glass::Parse(const YAML::Node& node)
{
	if (!node["Color"])
	{
		Error("Mirror material does not have color (line {})!", node.Mark().line + 1);
		return false;
	}
	try { m_Color = node["Color"].as<up<Texture>>(); }
	catch (...) { return false; }

	if (!node["Normal"])
	{
		Error("Mirror material does not have a normal map (line {})!", node.Mark().line + 1);
		return false;
	}
	try { m_Normal = node["Normal"].as<up<Texture>>(); }
	catch (...) { return false; }

	if (!node["Refractive Index"])
	{
		Error("Mirror material does not have an index of refraction (line {})!", node.Mark().line + 1);
		return false;
	}
	try { m_Eta = node["Refractive Index"].as<float>(); }
	catch (YAML::Exception& e)
	{
		Error("Mirror material index of refraction must be a float (line {})!", e.mark.line + 1);
		return false;
	}

	return true;
}
