#pragma once

#include "Shape.h"

class Object
{
public:
	Object() = default;

	Shape* ObjectShape = nullptr;
};

template<>
struct YAML::convert<Object>
{
	static bool decode(const Node& node, Object& object);
};
