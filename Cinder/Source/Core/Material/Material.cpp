#include "PCH.h"
#include "Core/Material/Material.h"

void Material::NormalMap(const up<Texture>& texture, Interaction& interaction) const
{
    Color c = texture->Evaluate(interaction);
    Vector normal = Vector(c.R, c.G, c.B).Normalize();
    interaction.SNormal = Normal(normal.TransformFrom(
        interaction.Tangent,
        interaction.Bitangent,
        Vector(interaction.GNormal)
    ));
}

bool YAML::convert<up<Material>>::decode(const Node& node, up<Material>& material)
{
	std::string type;
	try { type = node["Type"].as<std::string>(); }
	catch (YAML::Exception& e)
	{
		Error("Material type must be a string (line {})!", e.mark.line + 1);
		return false;
	}
	std::string name;
	try { name = node["Name"].as<std::string>(); }
	catch (YAML::Exception& e)
	{
		Error("Material name must be a string (line {})!", e.mark.line + 1);
		return false;
	}

	try { material = Registry::Get()->GMaterials.at(type)(name); }
	catch (...)
	{
		Error("Material type '{}' does not exist (line {})!", type, node["Type"].Mark().line + 1);
		return false;
	}

	return material->Parse(node);
}
