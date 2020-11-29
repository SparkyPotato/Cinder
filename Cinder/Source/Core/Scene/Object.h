#pragma once

#include "Shape.h"

class Object
{
public:
	Object() = default;

	bool Intersect(const Ray& ray, RayIntersection& intersection) const;
	bool TestIntersect(const Ray& ray) const;

	Shape* ObjectShape = nullptr;
};

template<>
struct YAML::convert<Object>
{
	static bool decode(const Node& node, Object& object);
};
