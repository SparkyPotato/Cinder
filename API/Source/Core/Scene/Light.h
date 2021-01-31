//    Copyright 2021 Shaye Garg
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

/// A tester for occlusion.
class Occlusion
{
public:
	Occlusion() = default;

	/// Test if the objects are occluded by the scene.
	///
	/// \return If there was an occlusion.
	bool operator()(const Scene& scene);

	/// The points to test for occlusion.
	Interaction Point1, Point2;
};

/// Base class for scene lighting.
class Light
{
public:
	/// Construct a light.
	///
	/// \param samples The amount of samples to take for the light.
	/// \param toCamera Local to camera transformation.
	/// \param isDelta Whether the light is characterizes by a delta distribution in space or direction.
	Light(uint32_t samples, const Transform& toCamera, bool isDelta)
		: SampleCount(samples), ToCamera(toCamera), IsDelta(isDelta)
	{}

	/// Virtual destructor.
	virtual ~Light() {}

	/// Sample the light.
	///
	/// \param interaction The interaction to sample the light for.
	/// \param sampler The Sampler to use.
	/// \param incoming The incoming vector to be filled by the light.
	/// \param pdf The probability distribution to be filled by the light.
	/// \param tester The occlusion tester to be filled by the light.
	/// 
	/// \return The incoming radiance at the interaction.
	virtual Color Sample(const Interaction& interaction, Sampler* sampler, Vector& incoming, float& pdf, Occlusion& tester) const = 0;

	/// Evaluate the radiance along a ray.
	///
	/// \param ray The ray to evaluate along.
	/// 
	/// \return The radiance along the ray.
	virtual Color EvaluateAlong(const Ray& ray) const = 0;

	/// Parse the required parameters.
	///
	/// \param node The node containing the data.
	/// 
	/// \return If the parse succeeded.
	virtual bool Parse(const YAML::Node& node) = 0;

	/// Preprocess before rendering.
	///
	/// \param scene The scene to preprocess for.
	virtual void Preprocess(const Scene& scene) {};

	Transform ToCamera;
	uint32_t SampleCount;
	bool IsDelta;
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
