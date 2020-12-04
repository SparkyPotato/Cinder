#include "PCH.h"
#include "Cameras/Perspective.h"

CAMERA(Perspective, PerspectiveCamera)

PerspectiveCamera::PerspectiveCamera(const Transform& transform)
	: Camera(transform)
{}

bool PerspectiveCamera::Parse(const YAML::Node& node)
{
	return true;
}

Ray PerspectiveCamera::GetRay(float u, float v)
{
	return Ray();
}
