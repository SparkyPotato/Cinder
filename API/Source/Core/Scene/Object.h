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

#include "Core/Math/Transform.h"
#include "Core/Scene/Geometry.h"

/// An object present in a scene.
class Object
{
public:
	Object() = default;
	
	/// Intersect a ray with the object.
	///
	/// \param ray The ray to intersect.
	/// \param interaction The interaction to fill in (in camera space).
	/// 
	/// \return If there was a hit.
	bool Intersect(const Ray& ray, Interaction& interaction) const;

	/// Test intersect a ray with the object.
	///
	/// \param ray The ray to intersect.
	/// 
	/// \return If there was a hit.
	bool TestIntersect(const Ray& ray) const;
	
	/// Get the bounding box of the object.
	///
	/// \return The bounding box in world space.
	Bound GetBound() { return ToCamera(m_Geometry->GetBound()); }

	/// Get the geometry of the object.
	///
	/// \return The geometry the object consists off.
	const Geometry* GetGeometry() const { return m_Geometry; }

	/// Sample a point on the object.
	///
	/// \param sampler The Sampler to use to sample.
	/// \param pdf The probability distribution of the sample.
	/// 
	/// \return The Interaction at the sampled point.
	Interaction Sample(Sampler* sampler, float& pdf) const;
	
	/// Get the material of the object.
	///
	/// \return The material.
	const Material* GetMaterial() const { return m_Material; }

	Transform ToCamera;

private:
	friend struct YAML::convert<Object>;
	friend class Scene;
	
	std::string m_GeometryName;
	std::string m_MaterialName;
	
	Geometry* m_Geometry = nullptr;
	Material* m_Material = nullptr;
};

template<>
struct YAML::convert<Object>
{
	static bool decode(const Node& node, Object& object);
};
