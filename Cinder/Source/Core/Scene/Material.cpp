#include "PCH.h"
#include "Material.h"

#include "Core/Components/TextureLoader.h"

Color Material::SampleAlbedo(float u, float v)
{
	return Sampler->Sample(Albedo, u, v);
}

bool LoadTexture(Texture& texture, const YAML::Node& node);

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

		material.Albedo.Height = material.Albedo.Width = 1;
		material.Albedo.Data.emplace_back(color);
	}
	else
	{
		if (!LoadTexture(material.Albedo, node["Albedo"])) { return false; }
	}
	
	return true;
}

bool LoadTexture(Texture& texture, const YAML::Node& node)
{
	std::string loader = "Standard";
	if (!node["Loader"])
	{
		Warning("No explicit loader! Using default '{}' (line {})", loader, node.Mark().line + 1);
	}
	if (!node["File"])
	{
		Error("No provided file (line {})!", node.Mark().line + 1);
		return false;
	}

	try { loader = node["Loader"].as<std::string>(); }
	catch (YAML::Exception& e)
	{
		Error("Loader must be a string (line {})!", e.mark.line + 1);
		return false;
	}
	std::string file;
	try { file = node["File"].as<std::string>(); }
	catch (YAML::Exception& e)
	{
		Error("File must be a string (line {})!", e.mark.line + 1);
		return false;
	}

	TextureLoader* tLoader;
	try { tLoader = ComponentManager::Get()->SpawnTextureLoader(loader); }
	catch (...)
	{
		Error("Texture Loader '{}' does not exist (line {})!", loader, node["Loader"].Mark().line + 1);
		return false;
	}

	try { texture = tLoader->LoadTexture(file); }
	catch (...)
	{
		Error("Failed to load texture '{}' (line {})!", file, node["File"].Mark().line + 1);
		return false;
	}

	return true;
}
