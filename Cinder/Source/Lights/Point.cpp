#include "PCH.h"
#include "Lights/Point.h"

LIGHT(Point, PointLight)

PointLight::PointLight(const Transform& transform)
	: Light(transform)
{}

Color PointLight::EvaluateSample(const RayIntersection& intersection, const std::pair<float, float>& sample, Vector& incoming, float& pdf, Occlusion& tester) const
{
	incoming = (m_Position - intersection.HitPoint).Normalize();
	pdf = 1.f;

	tester.Point1 = intersection;
	RayIntersection r;
	r.HitPoint = m_Position;
	tester.Point2 = r;

	return m_Color / (m_Position - intersection.HitPoint).GetLengthSquare();
}

bool PointLight::Parse(const YAML::Node& node)
{
	if (!node["Color"])
	{
		Error("Point light does not have color (line {})!", node.Mark().line + 1);
		return false;
	}

	try { m_Color = node["Color"].as<Color>(); }
	catch (...) { return false; }

	return true;
}

void PointLight::Preprocess(const Scene& scene)
{
	m_Position = ToCamera(Point());
}
