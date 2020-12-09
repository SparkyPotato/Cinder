#include "PCH.h"
#include "Materials/Matte.h"

#include "BxDFs/Lambertian.h"

MATERIAL(Matte, MatteMaterial)

MatteMaterial::MatteMaterial(const std::string& name)
	: Material(name)
{}

BSDF* MatteMaterial::GetBSDF(const RayIntersection& intersection, MemoryArena& arena) const
{
	auto bsdf = arena.Allocate<BSDF>(intersection, intersection.HitNormal);
	bsdf->Add(arena.Allocate<LambertianBRDF>(m_Color->Evaluate(intersection)), 1.f);

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

	return true;
}

