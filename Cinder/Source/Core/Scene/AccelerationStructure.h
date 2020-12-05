#pragma once

#include "Core/Scene/Object.h"
#include "Core/Scene/Scene.h"

class AccelerationStructure
{
public:
	virtual ~AccelerationStructure() {}

	virtual bool Parse(const YAML::Node& node) = 0;

	virtual bool Intersect(const Ray& ray, RayIntersection& intersection) = 0;
	virtual bool TestIntersect(const Ray& ray)
	{
		RayIntersection dummy;
		return Intersect(ray, dummy);
	}

	virtual void Build(const Scene& scene) = 0;
};

#define ACCELERATION(type, className) \
AccelerationStructure* Spawn##className() { return new className(); } \
struct Register##className \
{ \
	Register##className() { Registry::Get()->GAcceleration.emplace(#type, &Spawn##className); } \
}; \
static Register##className StaticRegister##className;
