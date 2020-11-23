#include "Material.h"

Material::Material(const Color& albedo)
	: m_Albedo(albedo)
{}

namespace YAML
{
bool convert<Material>::decode(const Node& node, Material& material)
{
	if (!node["Albedo"])
	{
		Error("Material does not have an albedo! ("
			COLOR, "line ", node.Mark().line, "\x1b[31m).");
	}

	material = Material(
		node["Albedo"].as<Color>()
	);

	return true;
}
}
