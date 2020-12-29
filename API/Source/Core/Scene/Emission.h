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
	const Object* Owner = nullptr;
};

#define EMISSION(type, className) \
up<Emission> Spawn##className(uint32_t s) { return std::make_unique<className>(s); } \
struct Register##className \
{ \
	Register##className() { Registry::Get()->GEmission.emplace(#type, &Spawn##className); } \
}; \
static Register##className StaticRegister##className;
