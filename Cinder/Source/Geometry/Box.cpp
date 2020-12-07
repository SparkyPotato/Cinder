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
}

Bound Box::GetBound() const
{
	return m_Bound;
}

bool Box::Intersect(const Ray& ray, RayIntersection& intersection) const
{
	
}
