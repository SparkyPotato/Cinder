#include "PCH.h"
#include "AccelerationStructures/None.h"

ACCELERATION(None, None)

bool None::Parse(const YAML::Node& node)
{
	return true;
}

bool None::Intersect(const Ray& ray, RayIntersection& intersection) const
{
	int ret = 0;

	for (const auto& object : *m_Objects)
	{
		if (object.Intersect(ray, intersection)) { ret++; }
	}

	return ret > 0;
}

bool None::TestIntersect(const Ray& ray) const
{
	for (const auto& object : *m_Objects)
	{
		if (object.TestIntersect(ray)) { return true; }
	}

	return false;
}

void None::Build(const Scene& scene)
{
	m_Objects = &scene.GetObjects();
}
