#include "PCH.h"
#include "ASNone.h"

REGISTER_ACCELERATION_STRUCTURE(None, ASNone)

void ASNone::Build(const Scene& scene)
{
	m_Scene = &scene;
}

bool ASNone::Intersect(const Ray& ray, RayIntersection& intersection)
{
	int ret = false;
	for (const auto& object : m_Scene->Objects)
	{
		if (object.Intersect(ray, intersection))
		{
			ret++;
		}
	}
	return ret;
}

bool ASNone::TestIntersect(const Ray& ray)
{
	for (const auto& object : m_Scene->Objects)
	{
		if (object.TestIntersect(ray)) { return true; }
	}
	return false;
}
