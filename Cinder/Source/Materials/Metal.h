#pragma once

#include "Core/Material/Material.h"

class Metal : public Material
{
public:
	Metal(const std::string& name);

	virtual void Compute(Interaction& interaction, MemoryArena& arena) const override;

	virtual bool Parse(const YAML::Node& node) override;

private:
	up<Texture> m_Color;
	up<Texture> m_Roughness;
	up<Texture> m_Eta, m_K;
};
