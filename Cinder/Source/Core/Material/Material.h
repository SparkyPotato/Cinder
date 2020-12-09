#pragma once

#include "Core/Material/BSDF.h"
#include "Core/Material/Texture.h"
#include "Core/Components/Registry.h"

class Material
{
public:
	Material(const std::string& name)
		: Name(name)
	{}
	virtual ~Material() = default;

	virtual BSDF* GetBSDF(const RayIntersection& intersection, MemoryArena& arena) const = 0;
	virtual bool Parse(const YAML::Node& node) = 0;

	std::string Name;
};

template<>
struct YAML::convert<up<Material>>
{
	static bool decode(const Node& node, up<Material>& material);
};

#define MATERIAL(type, className) \
up<Material> Spawn##className(const std::string& name) { return std::make_unique<className>(name); } \
struct Register##className \
{ \
Register##className() { Registry::Get()->GMaterials.emplace(#type, &Spawn##className); } \
}; \
static Register##className StaticRegister##className;
