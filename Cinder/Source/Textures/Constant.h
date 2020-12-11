#pragma once

#include "Core/Material/Texture.h"

class ConstantTexture : public Texture
{
public:
	virtual ~ConstantTexture() = default;
	
	virtual Color Evaluate(const Interaction& interaction) override;
	
	virtual bool Parse(const YAML::Node& node) override;
	
private:
	Color m_Color;
};
