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
		ray.Extent = t0;
		intersection.HitPoint = ray(t0);
		intersection.HitNormal = Normal();

		Vector diagonal = m_Bound.GetDiagonal() / 2.f;

		for (int i = 0; i < 3; i++)
		{
			if (IsNearlyEqual(std::abs(intersection.HitPoint[i]), diagonal[i]))
			{
				intersection.HitNormal[i] = std::copysign(1.f, intersection.HitPoint[i]);
				break;
			}
		}
		
		// TODO: Update intersection U and V
		
		return true;
	}
	
	return false;
}

bool Box::TestIntersect(const Ray& ray) const
{
	float t0, t1;
	return m_Bound.Intersect(ray, t0, t1);
}

float Box::GetArea() const
{
	return m_Bound.GetSurfaceArea();
}
