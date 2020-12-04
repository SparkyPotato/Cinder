#include "PCH.h"
#include "Geometry/Sphere.h"

GEOMETRY(Sphere, Sphere)

Sphere::Sphere(const std::string& name)
	: Geometry(name)
{}

bool Sphere::Parse(const YAML::Node& node)
{
	try { m_Radius = node["Radius"].as<float>(); }
	catch (YAML::Exception& e)
	{
		Error("Sphere radius must be a float (line {})!", e.mark.line + 1);
		return false;
	}

	return true;
}

bool Sphere::Intersect(const Ray& ray, RayIntersection& intersection)
{
	return false;
}

bool Sphere::TestIntersect(const Ray& ray)
{
	return false;
}
