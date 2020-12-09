#pragma once

#include "Core/Material/Material.h"

class MatteMaterial : public Material
{
public:
	MatteMaterial(const std::string& name);

	virtual BSDF* GetBSDF(const RayIntersection& intersection, MemoryArena& arena) const override;

	virtual bool Parse(const YAML::Node& node) override;

private:
	up<Texture> m_Color;
	up<Texture> m_Roughness;
};
