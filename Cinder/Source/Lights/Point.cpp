#include "PCH.h"
#include "Lights/Point.h"

LIGHT(Point, PointLight)

PointLight::PointLight(const Transform& transform)
	: Light(transform)
{}

Color PointLight::EvaluateSample(const Interaction& interaction, const std::pair<float, float>& sample, Vector& incoming, float& pdf, Occlusion& tester) const
{
	Vector d = m_Position - interaction.HitPoint;
	incoming = d.GetNormalized();
	pdf = 1.f;

	tester.Point1 = interaction;
	tester.Point2.HitPoint = m_Position;

	return m_Color / d.GetLengthSquare();
}

bool PointLight::Parse(const YAML::Node& node)
{
	if (!node["Color"])
	{
		Error("Point light does not have color (line {})!", node.Mark().line + 1);
		return false;
	}
	if (!node["Intensity"])
	{
		Error("Point light does not have intensity (line {})!", node.Mark().line + 1);
		return false;
	}

	try { m_Color = node["Color"].as<Color>(); }
	catch (...) { return false; }
	
    float intensity;
	try { intensity = node["Intensity"].as<float>(); }
	catch (YAML::Exception& e)
	{
		Error("Point light intensity must be a float (line {})!", e.mark.line + 1);
		return false;
	}
	
	m_Color *= intensity;

	return true;
}

void PointLight::Preprocess(const Scene& scene)
{
	m_Position = ToCamera(Point());
}
