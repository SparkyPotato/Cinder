#pragma once

#include "Core/Components/Registry.h"
#include "Core/Components/Framebuffer.h"
#include "Core/Scene/Scene.h"

class Renderer
{
public:
	virtual ~Renderer() {}

	virtual void Render(const Scene& scene, Framebuffer& framebuffer) = 0;
	virtual bool Parse(const YAML::Node& node) = 0;
};

template<>
struct YAML::convert<Renderer*>
{
	static bool decode(const Node& node, Renderer*& renderer);
};

#define RENDERER(type, className) \
Renderer* Spawn##className() { return new className; } \
struct Register##className \
{ \
	Register##className() { Registry::Get()->GRenderers.emplace(#type, &Spawn##className); } \
}; \
static Register##className StaticRegister##className;
