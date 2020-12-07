#pragma once

#include "Core/Components/Registry.h"
#include "Core/Math/Ray.h"
#include "Core/Math/RayIntersection.h"

class Geometry
{
public:
	Geometry(const std::string name)
		: Name(name)
	{}
	virtual ~Geometry() = default;

	virtual bool Parse(const YAML::Node& node) = 0;

	virtual bool Intersect(const Ray& ray, RayIntersection& intersection) = 0;
	virtual bool TestIntersect(const Ray& ray)
	{
		RayIntersection dummy;
		return Intersect(ray, dummy);
	}

	std::string Name;
};

template<>
struct YAML::convert<up<Geometry>>
{
	static bool decode(const Node& node, up<Geometry>&);
};

#define GEOMETRY(type, className) \
up<Geometry> Spawn##className(const std::string& name) { return std::make_unique<className>(name); } \
struct Register##className \
{ \
	Register##className() { Registry::Get()->GGeometry.emplace(#type, &Spawn##className); } \
}; \
static Register##className StaticRegister##className;
