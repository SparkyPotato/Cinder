#include "PCH.h"
#include "Sphere.h"

REGISTER_SHAPE(Sphere, Sphere)

Sphere::Sphere(const Transform& objectToCamera)
	: Shape(objectToCamera)
{}

bool Sphere::Intersect(const Ray& ray, RayIntersection& intersection)
{
	// Transform to object space
	Ray transformed = m_ObjectToCamera.GetInverse()(ray);

	// Get Quadratic coefficients
	float a = transformed.Direction.X * transformed.Direction.X +
		transformed.Direction.Y * transformed.Direction.Y +
		transformed.Direction.Z * transformed.Direction.Z;
	float b = 2 * (transformed.Direction.X * transformed.Origin.X +
		transformed.Direction.Y * transformed.Origin.Y +
		transformed.Direction.Z * transformed.Origin.Z);
	float c = transformed.Origin.X * transformed.Origin.X +
		transformed.Origin.Y * transformed.Origin.Y +
		transformed.Origin.Z * transformed.Origin.Z -
		m_Radius * m_Radius;

	float t0, t1, t;
	if (!SolveQuadratic(a, b, c, t0, t1)) { return false; }
	t = (t0 > 0.f) ? t0 : t1;
	if (t >= ray.Extent) { return false; }

	ray.Extent = t;
	Point point = ray.Origin + ray.Direction * t;
	intersection.HitPoint = point;
	intersection.HitNormal = Normal(intersection.HitPoint - Point()).GetNormalized();
	
	float azimuthal = std::atan2(-point.X, point.Z);
	float polar = std::acos(-point.Y / m_Radius);
	intersection.U = 1.f - (azimuthal / (2 * Pi) + 0.5f);
	intersection.V = (1.f - polar / Pi);
	
	intersection.HitPoint = m_ObjectToCamera(intersection.HitPoint);
	intersection.HitNormal = m_ObjectToCamera(intersection.HitNormal);

	return true;
}

bool Sphere::TestIntersect(const Ray& ray)
{
	// Transform to object space
	Ray transformed = m_ObjectToCamera.GetInverse()(ray);
	
	// Get Quadratic coefficients
	float a = transformed.Direction.X * transformed.Direction.X +
		transformed.Direction.Y * transformed.Direction.Y +
		transformed.Direction.Z * transformed.Direction.Z;
	float b = 2 * (transformed.Direction.X * transformed.Origin.X +
		transformed.Direction.Y * transformed.Origin.Y +
		transformed.Direction.Z * transformed.Origin.Z);
	float c = transformed.Origin.X * transformed.Origin.X +
		transformed.Origin.Y * transformed.Origin.Y +
		transformed.Origin.Z * transformed.Origin.Z -
		m_Radius * m_Radius;

	// Check if there are real solutions
	return TestQuadratic(a, b, c);
}

bool Sphere::ParseProperties(const YAML::Node& node)
{
	if (!node["Radius"])
	{
		Error("Sphere has no radius (line {})!", node.Mark().line + 1);
		return false;
	}

	try { m_Radius = node["Radius"].as<float>(); }
	catch (YAML::Exception& e)
	{
		Error("Sphere Radius must be a float (line {})!", e.mark.line + 1);
		return false;
	}

	return true;
}
