#include "PCH.h"
#include "JPEGOutput.h"

#define STBIW_WINDOWS_UTF8
#include "stb_image_write.h"

REGISTER_OUTPUT_ADAPTER(JPEG, JPEGOuput)

JPEGOuput::JPEGOuput(const std::string& filename)
	: OutputAdapter(filename)
{}

void JPEGOuput::WriteOutput(const Framebuffer& framebuffer)
{
	struct Pixel
	{
		uint8_t R, G, B;
	};
	uint32_t size = framebuffer.Width * framebuffer.Height;
	auto buffer = new Pixel[size];
	const Color* data = framebuffer.GetOutputColorData();

	for (uint32_t i = 0; i < size; i++)
	{
		buffer[i].R = uint8_t(std::min(data[i].R, 1.f) * 255.f);
		buffer[i].G = uint8_t(std::min(data[i].G, 1.f) * 255.f);
		buffer[i].B = uint8_t(std::min(data[i].B, 1.f) * 255.f);
	}

	stbi_write_jpg(m_OutputFile.string().c_str(),
		int(framebuffer.Width), int(framebuffer.Height),
		3, // RGB
		buffer,
		m_Quality
	);

	delete[] buffer;
}

bool JPEGOuput::ParseSettings(const YAML::Node& node)
{
	if (node["Quality"]) { m_Quality = node["Quality"].as<int>(); }
	else
	{
		Warning("JPEG Quality not set! Using default ({})", m_Quality);
	}

	if (m_Quality < 0 || m_Quality > 100)
	{
		Error("Quality ({}) out of range! Must be between 0 to 100 inclusive.", m_Quality);
		return false;
	}

	return true;
}
