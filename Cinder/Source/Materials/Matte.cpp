#include "PCH.h"
#include "Materials/Matte.h"

#include "BxDFs/OrenNayar.h"

MATERIAL(Matte, MatteMaterial)

MatteMaterial::MatteMaterial(const std::string& name)
	: Material(name)
{}

void MatteMaterial::Compute(Interaction& interaction, MemoryArena& arena) const
{
	Vector gX, gZ;
	GenerateCoordinateSystem(Vector(interaction.GNormal), gX, gZ);
	Color n = m_Normal->Evaluate(interaction);
	interaction.SNormal = Normal(Vector(n.R, n.B, n.G).TransformFrom(gX, Vector(interaction.GNormal), gZ));
	
	interaction.Bsdf = arena.Allocate<BSDF>(interaction);
	interaction.Bsdf->Add(arena.Allocate<OrenNayar>(m_Color->Evaluate(interaction), m_Roughness->Evaluate(interaction).R), 1.f);
}

bool MatteMaterial::Parse(const YAML::Node& node)
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
	
	if (!node["Normal"])
	{
		Error("Matte Material does not have a normal map (line {})!", node.Mark().line + 1);
		return false;
	}
	try { m_Normal = node["Normal"].as<up<Texture>>(); }
	catch (...) { return false; }

	return true;
}

