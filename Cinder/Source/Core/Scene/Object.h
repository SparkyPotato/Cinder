#pragma once

#include "Core/Math/Transform.h"
#include "Core/Scene/Geometry.h"

class Object
{
public:
	Object() = default;
	
	bool Intersect(const Ray& ray, RayIntersection& intersection) const;
	bool TestIntersect(const Ray& ray) const;
	
private:
	friend struct YAML::convert<Object>;
	friend class Scene;
	
	std::string m_GeometryName;
	
	Transform m_ToCamera;
	Geometry* m_Geometry = nullptr;
};

template<>
struct YAML::convert<Object>
{
	static bool decode(const Node& node, Object& object);
};
