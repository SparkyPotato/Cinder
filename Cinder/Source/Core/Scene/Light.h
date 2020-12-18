#pragma once

#include "Core/Components/Registry.h"
#include "Core/Math/Transform.h"
#include "Core/Scene/Interaction.h"
#include "Core/Math/Color.h"

class Scene;

class Occlusion
{
public:
	Occlusion() = default;

	bool operator()(const Scene& scene);

	Interaction Point1, Point2;
};

class Light
{
public:
	Light(uint32_t samples, const Transform& toCamera)
		: SampleCount(samples), ToCamera(toCamera)
	{}
	virtual ~Light() {}

	virtual Color EvaluateSample(const Interaction& interaction, const std::pair<float, float>& sample, Vector& incoming, float& pdf, Occlusion& tester) const = 0;
	virtual Color EvaluateAlong(const Ray& ray) const = 0;

	virtual bool Parse(const YAML::Node& node) = 0;

	virtual void Preprocess(const Scene& scene) {};

	Transform ToCamera;
    uint32_t SampleCount;
};

template<>
struct YAML::convert<up<Light>>
{
	static bool decode(const Node& node, up<Light>& light);
};

#define LIGHT(type, className) \
up<Light> Spawn##className(uint32_t s, const Transform& t) { return std::make_unique<className>(s, t); } \
struct Register##className \
{ \
	Register##className() { Registry::Get()->GLights.emplace(#type, &Spawn##className); } \
}; \
static Register##className StaticRegister##className;
