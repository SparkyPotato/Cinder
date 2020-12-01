#include "PCH.h"
#include "Material.h"

bool YAML::convert<Material>::decode(const Node& node, Material& material)
{
	if (!node["Name"])
	{
		Error("Material must have a Name (line {})!", node.Mark().line + 1);
		return false;
	}
	
	try { material.Name = node["Name"].as<std::string>(); }
	catch (YAML::Exception& e)
	{
		Error("Material Name must be a string (line {})!", e.mark.line + 1);
		return false;
	}
	
	if (!node["Albedo"])
	{
		Error("Material must have an Albedo (line {})!", node.Mark().line + 1);
		return false;
	}
	
	try { material.Albedo = node["Albedo"].as<Color>(); }
	catch (YAML::Exception& e)
	{
		Error("Material Albedo must be a color (line {})!", e.mark.line + 1);
		return false;
	}
	
	return true;
}
