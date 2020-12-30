//    Copyright 2021 SparkyPotato
//
//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
//    
//    http://www.apache.org/licenses/LICENSE-2.0
//    
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.

#pragma once

#include "Core/Components/Registry.h"
#include "Core/Components/Sampler.h"
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

	virtual Color Sample(const Interaction& interaction, Sampler* sampler, Vector& incoming, float& pdf, Occlusion& tester) const = 0;
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
