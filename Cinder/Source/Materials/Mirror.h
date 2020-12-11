#pragma once

#include "Core/Material/Material.h"

class Mirror : public Material
{
public:
	Mirror(const std::string& name);

	virtual void Compute(Interaction& interaction, MemoryArena& arena) const override;

	virtual bool Parse(const YAML::Node& node) override;

private:
	up<Texture> m_Color;
	up<Texture> m_Normal;
};
