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

#include "Core/Math/Transform.h"
#include "Core/Scene/Geometry.h"
#include "Core/Scene/Emission.h"

class Object
{
public:
	Object() = default;
	
	bool Intersect(const Ray& ray, Interaction& interaction) const;
	bool TestIntersect(const Ray& ray) const;
	
	Bound GetBound() { return ToCamera(m_Geometry->GetBound()); }
	
	const Material* GetMaterial() const { return m_Material; }
	const Emission* GetEmission() const { return m_Emission.get(); }
	const Geometry* GetGeometry() const { return m_Geometry; }

	Transform ToCamera;

private:
	friend struct YAML::convert<Object>;
	friend class Scene;
	
	std::string m_GeometryName;
	std::string m_MaterialName;
	
	Geometry* m_Geometry = nullptr;
	Material* m_Material = nullptr;
	up<Emission> m_Emission;
};

template<>
struct YAML::convert<Object>
{
	static bool decode(const Node& node, Object& object);
};
