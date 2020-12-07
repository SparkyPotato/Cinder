#pragma once

#include "Core/Components/Registry.h"
#include "Core/Components/Framebuffer.h"
#include "Core/Scene/Scene.h"

class Renderer
{
public:
	virtual ~Renderer() = default;

	virtual void Render(const Scene& scene, Framebuffer& framebuffer) = 0;
	virtual bool Parse(const YAML::Node& node) = 0;
};

template<>
struct YAML::convert<up<Renderer>>
{
	static bool decode(const Node& node, up<Renderer>& renderer);
};

#define RENDERER(type, className) \
up<Renderer> Spawn##className() { return std::make_unique<className>(); } \
struct Register##className \
{ \
	Register##className() { Registry::Get()->GRenderers.emplace(#type, &Spawn##className); } \
}; \
static Register##className StaticRegister##className;
