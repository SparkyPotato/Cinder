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
		Error("stb image fail: {}", filePath, stbi_failure_reason());
		throw -1;
	}
	
	Texture ret(width, height);
	ret.Data = new Color[width * height];
	
	for (int i = 0; i < width * height; i ++)
	{
		float r, g, b;
		r = float(data[3 * i]) / 255.f;
		g = float(data[3 * i + 1]) / 255.f;
		b = float(data[3 * i + 2]) / 255.f;

		ret.Data[i] = Color(r, g, b);
	}
	
	stbi_image_free(data);

	return ret;
}
