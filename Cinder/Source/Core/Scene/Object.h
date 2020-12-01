#pragma once

#include "Core/Components/Shape.h"
#include "Material.h"

class Object
{
public:
	Object() = default;

	bool Intersect(const Ray& ray, RayIntersection& intersection) const;
	bool TestIntersect(const Ray& ray) const;

	Shape* ObjectShape = nullptr;
	Material* ObjectMaterial = nullptr;
	std::string MaterialName;
};

template<>
struct YAML::convert<Object>
{
	static bool decode(const Node& node, Object& object);
};
