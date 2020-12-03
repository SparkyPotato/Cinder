#pragma once

#include "ComponentManager.h"
#include "Core/Scene/Scene.h"

class Renderer
{
public:
	virtual ~Renderer() {}

	virtual bool ParseSettings(const YAML::Node& node) { return true; }

	virtual void Render(const Scene& scene, Framebuffer& framebuffer) = 0;
};

#define REGISTER_RENDERER(name, className) \
Renderer* Spawn##className() { return Memory::Get()->Allocate<className>(); } \
struct Register##className \
{ \
	Register##className() \
	{ \
		ComponentManager::Get()->RegisterRenderer(#name, &Spawn##className); \
	} \
}; \
static Register##className StaticRegister##className;
