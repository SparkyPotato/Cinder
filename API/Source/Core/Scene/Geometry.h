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
#include "Core/Math/Ray.h"
#include "Core/Scene/Interaction.h"
#include "Core/Math/Bound.h"
#include "Core/Components/Sampler.h"

/// Base class for scene geometry.
class Geometry
{
public:
	/// Construct geometry.
	///
	/// \param name Name of the geometry.
	Geometry(const std::string name)
		: Name(name)
	{}

	/// Virtual destructor.
	virtual ~Geometry() = default;

	/// Parse the required parameters.
	///
	/// \param node The node containing the data.
	/// 
	/// \return If the parse succeeded.
	virtual bool Parse(const YAML::Node& node) = 0;

	/// Get the bounding box of the geometry.
	///
	/// \return The bounding box in local space.
	virtual const Bound& GetBound() const = 0;

	/// Intersect a ray with the geometry.
	///
	/// \param ray The ray to intersect.
	/// \param interaction The interaction to fill in (in local space).
	/// 
	/// \return If there was a hit.
	virtual bool Intersect(const Ray& ray, Interaction& interaction) const = 0;

	/// Test intersect a ray with the geometry.
	///
	/// \param ray The ray to intersect.
	/// 
	/// \return If there was a hit.
	virtual bool TestIntersect(const Ray& ray) const
	{
		Interaction dummy;
		return Intersect(ray, dummy);
	}

	/// Get the area of the geometry.
	///
	/// \return The surface area.
	virtual float GetArea() const = 0;

	/// Get the sub-geometry .
	///
	/// \return std::vector of subgeometry.
	virtual const std::vector<Geometry*>& GetSubGeometry() = 0;

	/// Sample a point on the geometry.
	///
	/// \param sampler The Sampler to use to sample.
	/// \param pdf The probability distribution of the sample.
	/// 
	/// \return The Interaction at the sampled point.
	virtual Interaction Sample(Sampler* sampler, float& pdf) const = 0;

	std::string Name;
};

template<>
struct YAML::convert<up<Geometry>>
{
	static bool decode(const Node& node, up<Geometry>&);
};

#define GEOMETRY(type, className) \
up<Geometry> Spawn##className(const std::string& name) { return std::make_unique<className>(name); } \
struct Register##className \
{ \
	Register##className() { Registry::Get()->GGeometry.emplace(#type, &Spawn##className); } \
}; \
static Register##className StaticRegister##className;
