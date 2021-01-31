//    Copyright 2021 Shaye Garg
//
//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
//    
//    http://www.apache.org/licenses/LICENSE-2.0
//    
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.

#include "PCH.h"
#include "Textures/HDRTexture.h"

#include "stb_image.h"

TEXTURE(HDR, HDRTexture)

HDRTexture::HDRTexture(Color* data, uint32_t width, uint32_t height)
	: m_Width(width), m_Height(height)
{
	m_Data = new Color[width * height];

	for (uint32_t i = 0; i < width * height; i++)
	{
		m_Data[i] = data[i];
	}
}

HDRTexture::~HDRTexture()
{
	delete m_Data;
}

Color HDRTexture::Evaluate(const Interaction& interaction) const
{
	float u = interaction.U;
	float v = interaction.V;

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

const Color& HDRTexture::GetPixel(uint32_t x, uint32_t y) const
{
	return m_Data[y * m_Width + x];
}

bool HDRTexture::Parse(const YAML::Node& node)
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

	m_Data = new Color[width * height];
	for (int i = 0; i < width * height; i++)
	{
		auto c = Color(
			float(data[i * 3]) / 255.f,
			float(data[i * 3 + 1]) / 255.f,
			float(data[i * 3] + 2) / 255.f
		);

		m_Data[i] = Color(std::pow(c.R(), 2.2f), std::pow(c.G(), 2.2f), std::pow(c.B(), 2.2f));
	}

	stbi_image_free(data);

	return true;
}
