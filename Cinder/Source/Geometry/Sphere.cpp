#include "PCH.h"
#include "Geometry/Sphere.h"

#include "Core/Math/Sampling.h"

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

    m_Bound = Bound(
		Point(-m_Radius, -m_Radius, -m_Radius),
		Point(m_Radius, m_Radius, m_Radius)
	);

	m_Area = 4 * Pi * m_Radius * m_Radius;

	return true;
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
	interaction.GNormal = Normal(hit - Point()).GetNormalized();

    interaction.Tangent = Cross(Vector(0.f, 1.f, 0.f), hit - Point()).Normalize();
    interaction.Bitangent = Cross(interaction.Tangent, Vector(interaction.GNormal));

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
	return m_Area;
}

Point Sphere::Sample(Sampler* sampler, float& pdf) const
{
	pdf = 1.f / m_Area;
	return Point() + UniformSampleSphere(sampler->Get2D()) * m_Radius;
}
