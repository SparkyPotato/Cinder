#pragma once

#include "Core/Material/Material.h"

class Plastic : public Material
{
public:
	Plastic(const std::string& name);

	virtual void Compute(Interaction& interaction, MemoryArena& arena) const override;

	virtual bool Parse(const YAML::Node& node) override;

private:
	up<Texture> m_DiffuseColor, m_SpecularColor;
    up<Texture> m_Normal, m_Roughness;
	up<Texture> m_Eta;
};
