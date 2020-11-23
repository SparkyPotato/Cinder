#pragma once

#include "Globals.h"
#include "Math/Color.h"

class Material
{
public:
	Material() = default;
	Material(const Color& albedo);

private:
	Color m_Albedo;
};

namespace YAML
{
template<>
struct convert<Material>
{
	static bool decode(const Node& node, Material& material);
};
}
