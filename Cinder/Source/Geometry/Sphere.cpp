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
	Point hit;

	float a = Dot(ray.Direction, ray.Direction);
	auto origin = Vector(ray.Origin.X(), ray.Origin.Y(), ray.Origin.Z());
	float b = 2 * Dot(ray.Direction, origin);
	float c = Dot(origin, origin) - m_Radius * m_Radius;

	float t0, t1;
	if (!SolveQuadratic(a, b, c, t0, t1)) { return false; }
	if (t0 > ray.Extent || t1 <= 0.f) { return false; }
	float t = t0;
	if (t <= 0.f) { t = t1; }

	hit = ray(t);
	float phi = std::atan2(hit.X(), hit.Z());
	if (phi <= 0.f) { phi += 2 * Pi; }
	intersection.U = phi / (2 * Pi);
	float theta = std::acos(hit.Z() / m_Radius);
	intersection.V = theta * InversePi;

	intersection.HitPoint = hit;
	intersection.HitNormal = Normal(hit - Point()).GetNormalized();

	return true;
}

bool Sphere::TestIntersect(const Ray& ray)
{
	float a = Dot(ray.Direction, ray.Direction);
	auto origin = Vector(ray.Origin.X(), ray.Origin.Y(), ray.Origin.Z());
	float b = 2 * Dot(ray.Direction, origin);
	float c = Dot(origin, origin) - m_Radius * m_Radius;

	float t0, t1;
	if (!SolveQuadratic(a, b, c, t0, t1)) { return false; }
	if (t0 > ray.Extent || t1 <= 0.f) { return false; }

	return true;
}
