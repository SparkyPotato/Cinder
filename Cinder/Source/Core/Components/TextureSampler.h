#pragma once

#include "Core/Scene/Texture.h"
#include "ComponentManager.h"

class TextureSampler
{
public:
	virtual  Color Sample(const Texture& texture, float u, float v) = 0;
};

#define REGISTER_TEXTURE_SAMPLER(name, className) \
TextureSampler* Spawn##className() { return Memory::Get()->Allocate<className>(); } \
struct Register##className \
{ \
	Register##className() \
	{ \
		ComponentManager::Get()->RegisterTextureSampler(#name, &Spawn##className); \
	} \
}; \
static Register##className StaticRegister##className;
