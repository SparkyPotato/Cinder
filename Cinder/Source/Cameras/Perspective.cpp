#include "PCH.h"
#include "Cameras/Perspective.h"

CAMERA(Perspective, PerspectiveCamera)

PerspectiveCamera::PerspectiveCamera(const Transform& transform)
	: Camera(transform)
{}

bool PerspectiveCamera::Parse(const YAML::Node& node)
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
	m_FOV = tan(m_FOV / 2);
	m_FOV = tan(m_FOV / 2);

	return true;
}

Ray PerspectiveCamera::GetRay(float u, float v)
{
	float Px = (2.f * u - 1.f) * m_FOV * m_AspectRatio;
	float Py = (1.f - 2.f * v) * m_FOV;

	return Ray(Point(), Vector(Px, Py, 1.f).GetNormalized());
}
