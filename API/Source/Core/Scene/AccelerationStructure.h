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

#include "Core/Scene/Object.h"
#include "Core/Scene/Scene.h"

/// Ray-scene intersection acceleration structure.
class AccelerationStructure
{
public:
	/// Virtual destructor.
	virtual ~AccelerationStructure() {}

	/// Parse the required parameters.
	///
	/// \param node The node containing the data.
	/// 
	/// \return If the parse succeeded.
	virtual bool Parse(const YAML::Node& node) = 0;

	/// Intersect a ray with the structure.
	///
	/// \param ray The ray to intersect.
	/// \param interaction The interaction to fill in.
	/// 
	/// \return If there was a hit.
	virtual bool Intersect(const Ray& ray, Interaction& interaction) const = 0;

	/// Test intersect a ray with the structure.
	///
	/// \param ray The ray to intersect.
	/// 
	/// \return If there was a hit.
	virtual bool TestIntersect(const Ray& ray) const
	{
		Interaction dummy;
		return Intersect(ray, dummy);
	}

	/// Build the acceleration structure from the scene.
	///
	/// \param scene The scene to build the structure for.
	virtual void Build(const Scene& scene) = 0;
};

#define ACCELERATION(type, className) \
up<AccelerationStructure> Spawn##className() { return std::make_unique<className>(); } \
struct Register##className \
{ \
	Register##className() { Registry::Get()->GAcceleration.emplace(#type, &Spawn##className); } \
}; \
static Register##className StaticRegister##className;
