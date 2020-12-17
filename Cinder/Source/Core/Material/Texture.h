#pragma once

#include "Core/Math/Color.h"
#include "Core/Scene/Interaction.h"
#include "Core/Components/Registry.h"

class Texture
{
public:
	virtual ~Texture() = default;
	
	virtual Color Evaluate(const Interaction& interaction) const = 0;
	
	virtual bool Parse(const YAML::Node& node) = 0;
};

template<>
struct YAML::convert<up<Texture>>
{
	static bool decode(const Node& node, up<Texture>& cubemap);
};

#define TEXTURE(type, className) \
up<Texture> Spawn##className() { return std::make_unique<className>(); } \
struct Register##className \
{ \
Register##className() { Registry::Get()->GTextures.emplace(#type, &Spawn##className); } \
}; \
static Register##className StaticRegister##className;
