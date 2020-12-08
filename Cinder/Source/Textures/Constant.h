#pragma once

#include "Core/Material/Texture.h"

class ConstantTexture : public Texture
{
public:
	virtual ~ConstantTexture() = default;
	
	virtual Color Evaluate(const RayIntersection& intersection) override;
	
	virtual bool Parse(const YAML::Node& node) override;
	
private:
	Color m_Color;
};
