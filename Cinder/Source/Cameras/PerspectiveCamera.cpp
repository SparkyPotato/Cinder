#include "PCH.h"
#include "PerspectiveCamera.h"

#include "Core/Components/Framebuffer.h"

REGISTER_CAMERA(Perspective, PerspectiveCamera)

bool PerspectiveCamera::ParseProperties(const YAML::Node& node)
{
	if (!node["FOV"])
	{
		Error("Camera does not have an FOV (line {})!", node.Mark().line + 1);
		return false;
	}
	
	try { m_FOV = node["FOV"].as<float>(); }
	catch (YAML::Exception& e)
	{
		Error("Camera FOV must be a float (line {})!", e.mark.line + 1);
		return false;
	}
	
	m_FOV = ToRadians(m_FOV);
	
	return true;
}

void PerspectiveCamera::RegisterFramebuffer(const Framebuffer& framebuffer)
{
	m_Framebuffer = &framebuffer;

	float aspectRatio = (float)m_Framebuffer->Width / m_Framebuffer->Height;
	m_AspectX = tan(m_FOV / 2) * aspectRatio;
	m_AspectY = tan(m_FOV / 2);
}

Ray PerspectiveCamera::GetRay(uint32_t x, uint32_t y)
{
	ASSERT(m_Framebuffer, "Camera not registered to any framebuffer!");

	float Px = (2.f * ((x + 0.5f) / m_Framebuffer->Width) - 1.f) * m_AspectX;
	float Py = (1.f - 2.f * ((y + 0.5f) / m_Framebuffer->Height)) * m_AspectY;

	return Ray(Point(), Vector(Px, Py, 1.f));
}
