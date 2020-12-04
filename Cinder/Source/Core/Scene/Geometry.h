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
	virtual ~Geometry() {}

	virtual bool Parse(const YAML::Node& node) = 0;

	virtual bool Intersect(const Ray& ray, RayIntersection& intersection) = 0;
	virtual bool TestIntersect(const Ray& ray) = 0;

	std::string Name;
};

template<>
struct YAML::convert<Geometry*>
{
	static bool decode(const Node& node, Geometry*&);
};

#define GEOMETRY(type, className) \
Geometry* Spawn##className(const std::string& name) { return Memory::Get()->Allocate<className>(name); } \
struct Register##className \
{ \
	Register##className() { Registry::Get()->GGeometry.emplace(#type, &Spawn##className); } \
}; \
static Register##className StaticRegister##className;
