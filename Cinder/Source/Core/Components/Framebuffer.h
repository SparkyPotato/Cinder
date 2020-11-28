#pragma once

#include "ComponentManager.h"
#include "Core/Math/Color.h"

class Framebuffer
{
public:
	Framebuffer(uint32_t width, uint32_t height)
		: Width(width), Height(height)
	{}

	virtual ~Framebuffer() {}

	uint32_t Width, Height;

	virtual const Color* GetOutputColorData() const = 0;
};

#define REGISTER_FRAMEBUFFER(name, className) \
Framebuffer* Spawn##className(uint32_t width, uint32_t height) { return new className(width, height); } \
struct Register##className \
{ \
	Register##className() \
	{ \
		ComponentManager::Get()->RegisterFramebuffer(#name, &Spawn##className); \
	} \
}; \
static Register##className StaticRegister##className;
