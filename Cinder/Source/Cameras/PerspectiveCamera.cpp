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
	
	if (m_FOV > 90.f)
	{
		Warning("FOV greater than 90 degrees may lead to distorted images.");
	}

	m_FOV = ToRadians(m_FOV);
	
	return true;
}

void PerspectiveCamera::SetAspectRatio(float aspectRatio)
{
	m_AspectX = tan(m_FOV / 2) * aspectRatio;
	m_AspectY = tan(m_FOV / 2);
}

Ray PerspectiveCamera::GetRay(float x, float y)
{
	ASSERT(m_AspectX > 0.f, "Camera Aspect Ratio not set!");

	float Px = (2.f * x - 1.f) * m_AspectX;
	float Py = (1.f - 2.f * y) * m_AspectY;

	return Ray(Point(), Vector(Px, Py, 1.f));
}
