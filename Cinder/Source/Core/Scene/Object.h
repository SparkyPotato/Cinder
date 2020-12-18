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
