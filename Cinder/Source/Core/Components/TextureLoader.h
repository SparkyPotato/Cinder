#pragma once

#include "ComponentManager.h"
#include "Core/Scene/Texture.h"

class TextureLoader : public Component
{
public:
	virtual ~TextureLoader() {}

	virtual bool ParseSettings(const YAML::Node& node) { return true; }
	virtual Texture LoadTexture(const std::string& filePath) = 0;
};

#define REGISTER_TEXTURE_LOADER(name, className) \
TextureLoader* Spawn##className() { return Memory::Get()->Allocate<className>(); } \
struct Register##className \
{ \
	Register##className() \
	{ \
		ComponentManager::Get()->RegisterTextureLoader(#name, &Spawn##className); \
	} \
}; \
static Register##className StaticRegister##className;
