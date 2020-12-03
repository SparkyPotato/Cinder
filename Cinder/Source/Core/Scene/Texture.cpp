#include "PCH.h"
#include "Texture.h"

#include "Core/Components/TextureLoader.h"

Texture::Texture(uint32_t width, uint32_t height)
	: Width(width), Height(height)
{}

const Color& Texture::GetPixel(uint32_t x, uint32_t y) const
{
	ASSERT(x < Width, "Sampling out of range!");
	ASSERT(y < Height, "Sampling out of range!");

	return Data[y * Height + x];
}

Color& Texture::GetPixel(uint32_t x, uint32_t y)
{
	ASSERT(x < Width, "Sampling out of range!");
	ASSERT(y < Height, "Sampling out of range!");

	return Data[y * Height + x];
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
