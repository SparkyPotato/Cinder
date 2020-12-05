#include "PCH.h"
#include "Framebuffers/8Bit.h"

#define STBIW_WINDOWS_UTF8
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

FRAMEBUFFER(8-bit, EightBitFramebuffer)

EightBitFramebuffer::EightBitFramebuffer(uint32_t width, uint32_t height)
	: Framebuffer(width, height), m_Arena(((width * height) / (GOptions.TileSize * GOptions.TileSize)) * sizeof(EightBitTile))
{
	m_Data = new Pixel[width * height];
}

EightBitFramebuffer::~EightBitFramebuffer()
{
	delete[] m_Data;
}

BufferTile* EightBitFramebuffer::GetBufferTile(uint32_t xmin, uint32_t xmax, uint32_t ymin, uint32_t ymax)
{
	return m_Arena.Allocate<EightBitTile>(m_Data, xmin, xmax, ymin, ymax, Width);
}

void EightBitFramebuffer::SaveBufferTile(BufferTile* tile)
{
	// Writing directly to buffer, so no saving required
}

bool EightBitFramebuffer::Ouput(const std::string& file)
{
	std::filesystem::path path = file;
	if (path.has_parent_path()) { std::filesystem::create_directories(path.parent_path()); }
	
	int i = stbi_write_png(file.c_str(), Width, Height, 3, m_Data, sizeof(Pixel) * Width);
	if (i == 0)
	{
		Error("Failed to write image.");
		return false;
	}
	
	return true;
}

bool EightBitFramebuffer::Parse(const YAML::Node& node)
{
	if (!node["Output"])
	{
		Error("Undefined Output (line {})!", node.Mark().line + 1);
		return false;
	}
	std::string type;
	try { type = node["Output"].as<std::string>(); }
	catch (YAML::Exception& e)
	{
		Error("Ouput type must be a string (line {})!", e.mark.line + 1);
		return false;
	}
	
	if (type != "PNG")
	{
		Error("Only PNG output is supported (line {})!", node["Ouput"].Mark().line + 1);
		return false;
	}
	
	return true;
}

EightBitTile::EightBitTile(Pixel* data, uint32_t xmin, uint32_t xmax, uint32_t ymin, uint32_t ymax, uint32_t stride)
	: m_Xmin(xmin), m_Xmax(xmax), m_Ymin(ymin), m_Ymax(ymax), m_Stride(stride)
{
	m_FramebufferData = data;
}

Color EightBitTile::GetPixel(uint32_t x, uint32_t y)
{
	ASSERT(x >= m_Xmin && x < m_Xmax, "Out of buffer tile range!");
	ASSERT(y >= m_Ymin && y < m_Ymax, "Out of buffer tile range!");
	
	Pixel& pixel = m_FramebufferData[m_Stride * y + x];
	
	return Color(
		float(pixel.R) / 255.f,
		float(pixel.G) / 255.f,
		float(pixel.B) / 255.f
	);
}

void EightBitTile::SetPixel(const Color& color, uint32_t x, uint32_t y)
{
	ASSERT(x >= m_Xmin && x < m_Xmax, "Out of buffer tile range!");
	ASSERT(y >= m_Ymin && y < m_Ymax, "Out of buffer tile range!");
	
	Pixel& pixel = m_FramebufferData[m_Stride * y + x];
	
	pixel = {
		// Clamp all color values from 0.f - 1.f
		uint8_t(std::max(std::min(color.R, 1.f), 0.f) * 255.f),
		uint8_t(std::max(std::min(color.G, 1.f), 0.f) * 255.f),
		uint8_t(std::max(std::min(color.B, 1.f), 0.f) * 255.f)
	};
}
