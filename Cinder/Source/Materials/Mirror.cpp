#include "PCH.h"
#include "Materials/Mirror.h"

#include "Core/Material/Fresnel.h"
#include "BxDFs/Specular.h"

MATERIAL(Mirror, Mirror)

Mirror::Mirror(const std::string& name)
	: Material(name)
{}

void Mirror::Compute(Interaction& interaction, MemoryArena& arena) const
{
    NormalMap(m_Normal, interaction);
	interaction.Bsdf = arena.Allocate<BSDF>(interaction);

	auto fresnel = arena.Allocate<PerfectFresnel>();
	Color c = m_Color->Evaluate(interaction);
	interaction.Bsdf->Add(arena.Allocate<SpecularReflection>(c, fresnel));
}

bool Mirror::Parse(const YAML::Node& node)
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
		Error("Glass material does not have a normal map (line {})!", node.Mark().line + 1);
		return false;
	}
	try { m_Normal = node["Normal"].as<up<Texture>>(); }
	catch (...) { return false; }

	return true;
}
