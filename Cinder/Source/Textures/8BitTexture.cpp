#include "PCH.h"
#include "Textures/8BitTexture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

TEXTURE(8-bit, EightBitTexture)

EightBitTexture::~EightBitTexture()
{
	delete m_Data;
}


Color EightBitTexture::Evaluate(const RayIntersection& intersection)
{
	float u = intersection.U;
	float v = intersection.V;

	if (u > 1.f || v > 1.f || u < 0.f || v < 0.f)
	{
		return Color(1.f, 0.f, 0.f);
	}

	u *= m_Width - 1;
	v *= m_Height - 1;

	auto Lu = uint32_t(std::floor(u));
	auto Lv = uint32_t(std::floor(v));
	auto Hu = uint32_t(std::ceil(u));
	auto Hv = uint32_t(std::ceil(v));
	float uRatio = u - Lu;
	float vRatio = v - Lv;

	Color utLerp = Lerp(GetPixel(Lu, Lv), GetPixel(Hu, Lv), uRatio);
	Color ubLerp = Lerp(GetPixel(Lu, Hv), GetPixel(Hu, Hv), uRatio);

	return Lerp(utLerp, ubLerp, vRatio);
}

Color EightBitTexture::GetPixel(uint32_t x, uint32_t y)
{
	Pixel& pixel = m_Data[y * m_Width + x];

	return Color(float(pixel.R) / 255.f, float(pixel.G) / 255.f, float(pixel.B) / 255.f);
}

bool EightBitTexture::Parse(const YAML::Node& node)
{
	std::string file;
	try { file = node["Source"].as<std::string>(); }
	catch (YAML::Exception& e)
	{
		Error("Texture source must be a string (line {})!", e.mark.line + 1);
		return false;
	}

	int width, height, comp;
	uint8_t* data = stbi_load(file.c_str(), &width, &height, &comp, 3);

	m_Width = width; m_Height = height;

	m_Data = new Pixel[width * height];
	for (int i = 0; i < width * height; i++)
	{
		m_Data[i].R = data[i * 3 + 0];
		m_Data[i].G = data[i * 3 + 1];
		m_Data[i].B = data[i * 3 + 2];
	}

	stbi_image_free(data);

	return true;
}
