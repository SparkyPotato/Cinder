#include "Camera.h"

#include "Math/Math.h"

Camera::Camera(const Transform& transform, float verticalFOV)
	: m_Transform(transform), m_FOV(verticalFOV)
{}

namespace YAML
{
bool convert<Camera>::decode(const Node& node, Camera& camera)
{
	if (!node["Transform"]) 
	{ 
		Error("Camera does not have a transform! (" 
			COLOR, "line ", node.Mark().line, "\x1b[31m).");
	}
	if (!node["Vertical FOV"])
	{
		Error("Camera does not have a vertical FOV! ("
			COLOR, "line ", node.Mark().line, "\x1b[31m).");
	}

	camera = Camera(
		node["Transform"].as<Transform>(),
		ToRadians(node["Vertical FOV"].as<float>())
	);

	return true;
}
}
