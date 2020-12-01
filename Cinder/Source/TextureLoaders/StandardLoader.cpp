#include "PCH.h"
#include "StandardLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#define STBI_WINDOWS_UTF8
#include "stb_image.h"

REGISTER_TEXTURE_LOADER(Standard, StandardLoader)

Texture StandardLoader::LoadTexture(const std::string& filePath)
{
	int width, height, comp;
	uint8_t* data = stbi_load(filePath.c_str(), &width, &height, &comp, 3);
	if (!data)
	{
		Error("{}", filePath, stbi_failure_reason());
		throw -1;
	}
	
	Texture ret(width, height);
	
	for (int i = 0; i < width * height * 3; i += 3)
	{
		float r, g, b;
		r = float(data[i]) / 255.f;
		g = float(data[i + 1]) / 255.f;
		b = float(data[i + 2]) / 255.f;

		ret.Data.emplace_back(r, g, b);
	}

	return ret;
}
