#pragma once

#include "Core/Components/ComponentManager.h"
#include "Core/Math/Ray.h"

class Camera
{
public:
	virtual ~Camera() {}
	
	virtual bool ParseProperties(const YAML::Node& node) = 0;

	virtual void SetAspectRatio(float aspectRatio) = 0;
	virtual Ray GetRay(float x, float y) = 0;
};

#define REGISTER_CAMERA(name, className) \
Camera* Spawn##className() { return Memory::Get()->Allocate<className>(); } \
struct Register##className \
{ \
	Register##className() \
	{ \
		ComponentManager::Get()->RegisterCamera(#name, &Spawn##className); \
	} \
}; \
static Register##className StaticRegister##className;
