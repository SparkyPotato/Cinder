#include "PCH.h"
#include "Material.h"

Color Material::SampleAlbedo(float u, float v)
{
	return Sampler->Sample(Albedo, u, v);
}

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

	std::string sampler = "Bilinear";
	if (!node["Sampler"])
	{
		Warning("No Sampler specified,  using default '{}' (line {})!", sampler, node.Mark().line + 1);
	}

	try { sampler = node["Sampler"].as<std::string>(); }
	catch (YAML::Exception& e)
	{
		Error("Sampler must be a string (line {})!", e.mark.line + 1);
		return false;
	}

	try { material.Sampler = ComponentManager::Get()->SpawnTextureSampler(sampler); }
	catch (...)
	{
		Error("Sampler '{}' does not exist (line {})!", sampler, node["Sampler"].Mark().line + 1);
		return false;
	}
	
	if (!node["Albedo"])
	{
		Error("Material must have an Albedo (line {})!", node.Mark().line + 1);
		return false;
	}
	
	if (node["Albedo"].IsSequence())
	{
		Color color;
		try { color = node["Albedo"].as<Color>(); }
		catch (YAML::Exception& e)
		{
			Error("Invalid color albedo (line {})!", e.mark.line + 1);
			return false;
		}

		material.Albedo.Height = material.Albedo.Width = 2;
		material.Albedo.Data = Memory::Get()->AllocateArr<Color>(4);
		material.Albedo.Data[0] = color;
		material.Albedo.Data[1] = color;
		material.Albedo.Data[2] = color;
		material.Albedo.Data[3] = color;
	}
	else
	{
		if (!LoadTexture(material.Albedo, node["Albedo"])) { return false; }
	}
	
	return true;
}
