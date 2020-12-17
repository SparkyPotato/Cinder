#pragma once

#include "Core/Material/Material.h"

class MatteMaterial : public Material
{
public:
	MatteMaterial(const std::string& name);

	virtual void Compute(Interaction& interaction, MemoryArena& arena) const override;

	virtual bool Parse(const YAML::Node& node) override;

private:
	up<Texture> m_Color, m_Normal, m_Roughness;
};
