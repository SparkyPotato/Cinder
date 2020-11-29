#include "PCH.h"
#include "JPEGOutput.h"

#define STBIW_WINDOWS_UTF8
#include "stb_image_write.h"

REGISTER_OUTPUT_ADAPTER(JPEG, JPEGOuput)

JPEGOuput::JPEGOuput(const std::string& filename)
	: OutputAdapter(filename)
{
	Verbose("JPEG Output Adapter created with output file '{}'.", filename);
}

void JPEGOuput::WriteOutput(const Framebuffer& framebuffer)
{
	Log("Writing image to file.");

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
	if (node["Quality"]) 
	{
		try { m_Quality = node["Quality"].as<int>(); }
		catch (YAML::Exception& e)
		{
			Error("JPEG Quality must be an integer (line {})!", e.mark.line + 1);
			return false;
		}
	}
	else
	{
		Warning("JPEG Quality not set! Using default ({})", m_Quality);
	}

	if (m_Quality < 0 || m_Quality > 100)
	{
		Error("Quality ({}) out of range! Must be between 0 to 100 inclusive (line {}).", m_Quality, node["Quality"].Mark().line + 1);
		return false;
	}

	return true;
}
