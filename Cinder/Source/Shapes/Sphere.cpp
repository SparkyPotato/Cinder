#include "PCH.h"
#include "Sphere.h"

REGISTER_SHAPE(Sphere, Sphere)

Sphere::Sphere(const Transform& objectToWorld)
	: Shape(objectToWorld)
{}

bool Sphere::Intersect(const Ray&, RayIntersection& intersection)
{
	return false;
}

bool Sphere::TestIntersect(const Ray& ray)
{
	return false;
}

bool Sphere::ParseProperties(const YAML::Node& node)
{
	if (!node["Radius"])
	{
		Error("Sphere has no radius!");
		return false;
	}

	try { m_Radius = node["Radius"].as<float>(); }
	catch (YAML::Exception& e)
	{
		Error("Sphere Radius must be a float (line {})!", e.mark.line);
		return false;
	}

	return true;
}
