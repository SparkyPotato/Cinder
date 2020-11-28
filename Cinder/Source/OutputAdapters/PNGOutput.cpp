#include "PCH.h"
#include "PNGOutput.h"

#define STBIW_WINDOWS_UTF8
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

REGISTER_OUTPUT_ADAPTER(PNG, PNGOutput)

PNGOutput::PNGOutput(const std::string& filename)
	: OutputAdapter(filename)
{}

void PNGOutput::WriteOutput(const Framebuffer& framebuffer)
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

	stbi_write_png(m_OutputFile.string().c_str(), 
		int(framebuffer.Width), int(framebuffer.Height),
		3, // RGB
		buffer, 
		sizeof(Pixel) * int(framebuffer.Width)
	);
	
	delete[] buffer;
}
