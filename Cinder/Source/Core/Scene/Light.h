#pragma once

#include "Core/Components/Registry.h"
#include "Core/Math/Transform.h"
#include "Core/Math/RayIntersection.h"
#include "Core/Math/Color.h"

class Scene;

class Occlusion
{
public:
	Occlusion() = default;

	bool operator()(const Scene& scene);

	RayIntersection Point1, Point2;
};

class Light
{
public:
	Light(const Transform& toCamera)
		: ToCamera(toCamera)
	{}

	virtual Color EvaluateSample(const RayIntersection& intersection, const std::pair<float, float>& sample, Vector& incoming, float& pdf, Occlusion& tester) const = 0;

	virtual bool Parse(const YAML::Node& node) = 0;

	virtual void Preprocess(const Scene& scene) {};

	Transform ToCamera;
};

template<>
struct YAML::convert<up<Light>>
{
	static bool decode(const Node& node, up<Light>& light);
};

#define LIGHT(type, className) \
up<Light> Spawn##className(const Transform& t) { return std::make_unique<className>(t); } \
struct Register##className \
{ \
	Register##className() { Registry::Get()->GLights.emplace(#type, &Spawn##className); } \
}; \
static Register##className StaticRegister##className;