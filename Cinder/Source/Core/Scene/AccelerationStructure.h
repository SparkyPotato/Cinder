#pragma once

#include "Core/Components/ComponentManager.h"
#include "Ray.h"
#include "RayIntersection.h"
#include "Scene.h"

class AccelerationStructure
{
public:
	virtual ~AccelerationStructure() {}
	
	virtual void Build(const Scene& scene) = 0;

	virtual bool Intersect(const Ray& ray, RayIntersection& intersection) = 0;
	virtual bool TestIntersect(const Ray& ray) = 0;
};

#define REGISTER_ACCELERATION_STRUCTURE(name, className) \
AccelerationStructure* Spawn##className() { return new className; } \
struct Register##className \
{ \
	Register##className() \
	{ \
		ComponentManager::Get()->RegisterAccelerationStructure(#name, &Spawn##className); \
	} \
}; \
static Register##className StaticRegister##className;
