#pragma once

#include "Core/Math/Color.h"

class Material
{
public:
	Material() = default;
	
	std::string Name;
	Color Albedo;
};

template<>
struct YAML::convert<Material>
{
	static bool decode(const Node& node, Material& material);
};
