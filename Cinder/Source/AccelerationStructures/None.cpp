#include "PCH.h"
#include "AccelerationStructures/None.h"

ACCELERATION(None, None)

bool None::Parse(const YAML::Node& node)
{
	return true;
}

bool None::Intersect(const Ray& ray, RayIntersection& intersection)
{
	return false;
}

bool None::TestIntersect(const Ray& ray)
{
	return false;
}

void None::Build(const Scene& scene)
{
	m_Objects = &scene.GetObjects();
}
