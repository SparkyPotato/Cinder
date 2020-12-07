#include "PCH.h"
#include "Geometry/Box.h"

GEOMETRY(Box, Box)

Box::Box(const std::string& name)
	: Geometry(name)
{}

bool Box::Parse(const YAML::Node& node)
{
	float x, y, z;
	if (!node["Size"])
	{
		Error("Box must have a size (line {})!", node.Mark().line + 1);
		return false;
	}

	if (!node["Size"].IsSequence())
	{
		Error("Box size must be a sequence (line {})!", node["Size"].Mark().line + 1);
		return false;
	}

	try { x = node["Size"][0].as<float>(); }
	catch (YAML::Exception& e)
	{
		Error("Size must be a float sequence (line {})!", e.mark.line + 1);
		return false;
	}

	y = node["Size"][1].as<float>();
	z = node["Size"][1].as<float>();

	m_Bound = Bound(
		Point(-x / 2.f, -y / 2.f, -z / 2.f),
		Point(x / 2.f, y / 2.f, z / 2.f)
	);
	
	return true;
}

Bound Box::GetBound() const
{
	return m_Bound;
}

bool Box::Intersect(const Ray& ray, RayIntersection& intersection) const
{
	float t0, t1;
	bool hit = m_Bound.Intersect(ray, t0, t1);
	if (hit)
	{
		intersection.HitPoint = ray(t0);
		intersection.HitNormal = Normal();
		float max = 0.f;
		int i = 0;
		for (int j = 0; j < 3; j++)
		{
			if (std::abs(intersection.HitPoint[j]) > std::abs(max))
			{ max = intersection.HitPoint[j]; i = j; }
		}
		intersection.HitNormal[i] = max / std::abs(max);
		
		// TODO: Update intersection U and V
		
		return true;
	}
	
	return false;
}
