#include "PCH.h"
#include "Materials/Matte.h"

#include "BxDFs/OrenNayar.h"

MATERIAL(Matte, MatteMaterial)

MatteMaterial::MatteMaterial(const std::string& name)
	: Material(name)
{}

BSDF* MatteMaterial::GetBSDF(const RayIntersection& intersection, MemoryArena& arena) const
{
	Vector gX, gZ;
	GenerateCoordinateSystem(Vector(intersection.HitNormal), gX, gZ);
	Color n = m_Normal->Evaluate(intersection);
	Vector normal = Vector(n.R, n.B, n.G);
	
	auto bsdf = arena.Allocate<BSDF>(intersection, Normal(normal.TransformFrom(gX, Vector(intersection.HitNormal), gZ)));
	bsdf->Add(arena.Allocate<OrenNayar>(m_Color->Evaluate(intersection), m_Roughness->Evaluate(intersection).R), 1.f);

	return bsdf;
}

bool MatteMaterial::Parse(const YAML::Node& node)
{
	if (!node["Color"])
	{
		Error("Matte Material does not have color (line {})!", node.Mark().line + 1);
		return false;
	}
	try { m_Color = node["Color"].as<up<Texture>>(); }
	catch (...) { return false; }

	if (!node["Roughness"])
	{
		Error("Matte Material does not have roughness (line {})!", node.Mark().line + 1);
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

