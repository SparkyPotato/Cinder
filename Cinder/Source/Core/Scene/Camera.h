#pragma once

#include "Core/Components/ComponentManager.h"
#include "Ray.h"

class Camera
{
public:
	virtual ~Camera() {}
	
	virtual bool ParseProperties(const YAML::Node& node) = 0;

	virtual void RegisterFramebuffer(const Framebuffer& framebuffer) = 0;
	virtual Ray GetRay(uint32_t x, uint32_t y) = 0;
};

#define REGISTER_CAMERA(name, className) \
Camera* Spawn##className() { return new className(); } \
struct Register##className \
{ \
	Register##className() \
	{ \
		ComponentManager::Get()->RegisterCamera(#name, &Spawn##className); \
	} \
}; \
static Register##className StaticRegister##className;
