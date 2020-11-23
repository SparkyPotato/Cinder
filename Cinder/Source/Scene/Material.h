#pragma once

#include "Globals.h"
#include "Math/Color.h"

class Material
{
public:
	Material() = default;
	Material(const Color& albedo);

	const Color& GetAlbedo() const { return m_Albedo; }

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
