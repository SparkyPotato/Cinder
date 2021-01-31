//    Copyright 2021 Shaye Garg
//
//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
//    
//    http://www.apache.org/licenses/LICENSE-2.0
//    
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.

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

Interaction Sphere::Sample(Sampler* sampler, float& pdf) const
{
	pdf = 1.f / m_Area;
	Interaction i;

	auto sample = sampler->Get2D();
	float y = 1.f - 2.f * sample.first;
	float r = std::sqrt(std::max(0.f, 1.f - y * y));
	float phi = 2 * Pi * sample.second;

	Vector n = Vector(r * std::cos(phi), y, r * std::sin(phi));
	i.HitPoint = Point() + n * m_Radius;
	i.GNormal = Normal(n);

	i.Tangent = Cross(Vector(0.f, 1.f, 0.f), n);
	i.Bitangent = Cross(i.Tangent, Vector(i.GNormal));
	
	i.U = phi / (2 * Pi);
	float theta = std::acos(n.Y());
	i.V = theta * InversePi;

	return i;
}
