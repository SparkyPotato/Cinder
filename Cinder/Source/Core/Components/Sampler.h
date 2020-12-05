#pragma once

#include "Core/Components/Registry.h"

class Sampler
{
public:
	virtual ~Sampler() {}

	virtual float GetSample(float value, float delta) = 0;
};

#define SAMPLER(type, className) \
Sampler* Spawn##className() { return new className(); } \
struct Register##className \
{ \
	Register##className() { Registry::Get()->GSamplers.emplace(#type, &Spawn##className); } \
}; \
static Register##className StaticRegister##className;
