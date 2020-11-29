#include "PCH.h"
#include "ASNone.h"

REGISTER_ACCELERATION_STRUCTURE(None, ASNone)

void ASNone::Build(const Scene& scene)
{
	m_Scene = &scene;
}

bool ASNone::Intersect(const Ray& ray, RayIntersection& intersection)
{
	bool ret = false;
	for (const auto& object : m_Scene->Objects)
	{
		ret = object.Intersect(ray, intersection);
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
