#include "PCH.h"
#include "PerspectiveCamera.h"

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
