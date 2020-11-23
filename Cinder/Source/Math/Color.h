#pragma once

#include "Globals.h"

class Color
{
public:
	Color();
	Color(float r, float g, float b);
	Color(const Color& other) = default;

public:
	float R, G, B;
};

namespace YAML
{
template<>
struct convert<Color>
{
	static bool decode(const Node& node, Color& material);
};
}
