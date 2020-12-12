#pragma once

#include "Core/Math/Transform.h"
#include "Core/Scene/Geometry.h"

class Object
{
public:
	Object() = default;
	
	bool Intersect(const Ray& ray, Interaction& interaction) const;
	bool TestIntersect(const Ray& ray) const;
	
	Bound GetBound() { return m_Geometry->GetBound(); }
	
	const Material* GetMaterial() const { return m_Material; }

private:
	friend struct YAML::convert<Object>;
	friend class Scene;
	
	std::string m_GeometryName;
	std::string m_MaterialName;
	
	Transform m_ToCamera;
	Geometry* m_Geometry = nullptr;
	Material* m_Material = nullptr;
};

template<>
struct YAML::convert<Object>
{
	static bool decode(const Node& node, Object& object);
};
