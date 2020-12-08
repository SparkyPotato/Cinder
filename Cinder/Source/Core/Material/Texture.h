#pragma once

#include "Core/Math/Color.h"
#include "Core/Math/RayIntersection.h"

class Texture
{
public:
	virtual ~Texture() = default;
	
	virtual Color Evaluate(const RayIntersection& intersection) = 0;
	
	virtual bool Parse(const YAML::Node& node) = 0;
};

#define TEXTURE(type, className) \
up<Texture> Spawn##className() { return std::make_unique<className>(); } \
struct Register##className \
{ \
Register##className() { Registry::Get()->GTextures.emplace(#type, &Spawn##className); } \
}; \
static Register##className StaticRegister##className;
