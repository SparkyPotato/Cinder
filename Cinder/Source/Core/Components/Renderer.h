#pragma once

#include "Core/Scene/Scene.h"
#include "ComponentManager.h"

class Renderer
{
public:
	virtual ~Renderer() {}
	virtual void Render(const Scene& scene) = 0;
};

#define REGISTER_RENDERER(name, className) \
Renderer* Spawn##className() { return new className; } \
struct Register##className \
{ \
	Register##className() \
	{ \
		ComponentManager::Get()->RegisterRenderer(#name, &Spawn##className); \
	} \
}; \
static Register##className StaticRegister##className;
