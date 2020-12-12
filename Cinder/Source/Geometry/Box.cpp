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

bool Box::Intersect(const Ray& ray, Interaction& interaction) const
{
	float t0, t1;
	if (!m_Bound.Intersect(ray, t0, t1)) { return false; }
	if (t0 > ray.Extent || t1 <= 0.f) { return false; }

	float t = t0;
	if (t <= 0.f) { t = t1; }

	ray.Extent = t;
	interaction.HitPoint = ray(t);
	interaction.GNormal = Normal();

	Vector diagonal = m_Bound.GetDiagonal() / 2.f;

	for (int i = 0; i < 3; i++)
	{
		if (IsNearlyEqual(std::abs(interaction.HitPoint[i]), diagonal[i]))
		{
			interaction.GNormal[i] = std::copysign(1.f, interaction.HitPoint[i]);
			break;
		}
	}

	interaction.SNormal = interaction.GNormal;

	// TODO: Update intersection U and V

	return true;
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
