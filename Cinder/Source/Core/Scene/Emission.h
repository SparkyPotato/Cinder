#pragma once

#include "Core/Math/Color.h"
#include "Core/Scene/Interaction.h"
#include "Core/Components/Sampler.h"

class Object;
class Scene;

class Occlusion
{
public:
	Occlusion() = default;

	bool operator()(const Scene& scene);

	Interaction Point1, Point2;
};

class Emission
{
public:
	Emission(uint32_t samples)
		: SampleCount(samples)
	{}

	virtual bool Parse(const YAML::Node& node) = 0;

	virtual Color Evaluate(const Interaction& interaction) const = 0;
	virtual Color Sample(const Interaction& interaction, Sampler* sampler, Vector& incoming, float& pdf, Occlusion& tester) const = 0;

	uint32_t SampleCount;
	const Object* Owner;
};

#define EMISSION(type, className) \
up<Emission> Spawn##className(uint32_t s) { return std::make_unique<className>(s); } \
struct Register##className \
{ \
	Register##className() { Registry::Get()->GEmission.emplace(#type, &Spawn##className); } \
}; \
static Register##className StaticRegister##className;
