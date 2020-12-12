#include "PCH.h"
#include "Geometry/Sphere.h"

GEOMETRY(Sphere, Sphere)

Sphere::Sphere(const std::string& name)
	: Geometry(name)
{}

bool Sphere::Parse(const YAML::Node& node)
{
	if (!node["Radius"])
	{
		Error("Sphere must have radius (line {})!", node.Mark().line + 1);
		return false;
	}

	try { m_Radius = node["Radius"].as<float>(); }
	catch (YAML::Exception& e)
	{
		Error("Sphere radius must be a float (line {})!", e.mark.line + 1);
		return false;
	}

	return true;
}

Bound Sphere::GetBound() const
{
	return Bound(
		Point(-m_Radius, -m_Radius, -m_Radius),
		Point(m_Radius, m_Radius, m_Radius)
	);
}

bool Sphere::Intersect(const Ray& ray, Interaction& interaction) const
{
	float a = Dot(ray.Direction, ray.Direction);
	auto origin = Vector(ray.Origin.X(), ray.Origin.Y(), ray.Origin.Z());
	float b = 2 * Dot(ray.Direction, origin);
	float c = Dot(origin, origin) - m_Radius * m_Radius;

	float t0, t1;
	if (!SolveQuadratic(a, b, c, t0, t1)) { return false; }
	if (t0 > ray.Extent || t1 <= 0.f) { return false; }
	float t = t0;
	if (t <= 0.f) { t = t1; }

	Point hit = ray(t);
	ray.Extent = t;
	float phi = std::atan2(hit.X(), hit.Z());
	if (phi <= 0.f) { phi += 2 * Pi; }
	interaction.U = phi / (2 * Pi);
	float theta = std::acos(hit.Y() / m_Radius);
	interaction.V = theta * InversePi;

	interaction.HitPoint = hit;
	interaction.GNormal = interaction.SNormal = Normal(hit - Point()).GetNormalized();

	return true;
}

bool Sphere::TestIntersect(const Ray& ray) const
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

float Sphere::GetArea() const
{
	return 4 * Pi * m_Radius * m_Radius;
}
