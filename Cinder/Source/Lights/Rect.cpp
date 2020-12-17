#include "PCH.h"
#include "Lights/Rect.h"

LIGHT(Rect, RectLight)

RectLight::RectLight(const Transform& transform)
	: Light(transform)
{}

Color RectLight::EvaluateSample(const Interaction& interaction, const std::pair<float, float>& sample, 
	Vector& incoming, float& pdf, Occlusion& tester) const
{
	float x = Lerp(-m_Size.X(), m_Size.X(), sample.first);
	float z = Lerp(-m_Size.z(), m_Size.z(), sample.second);
}

Color RectLight::EvaluateAlong(const Ray& ray) const
{

}

virtual bool RectLight::Parse(const YAML::Node& node)
{
	if (!node["Color"])
	{
		Error("Rect light does not have color (line {})!", node.Mark().line + 1);
		return false;
	}
	if (!node["Intensity"])
	{
		Error("Rect light does not have intensity (line {})!", node.Mark().line + 1);
		return false;
	}
	if (!node["Dimension"])
	{
		Error("Rect light does not have a dimension (line {})!", node.Mark().line + 1);
		return false;
	}

	try { m_Color = node["Color"].as<Color>(); }
	catch (...) { return false; }
	
	float intensity;
	try { intensity = node["Intensity"].as<float>(); }
	catch (YAML::Exception& e)
	{
		Error("Rect light intensity must be a float (line {})!", e.mark.line + 1);
		return false;
	}

	if (!node["Dimension"].IsSequence() || node["Dimension"].size() != 2)
	{
		Error("Rect light dimension must be 2-dimensional (line {})!", node.Mark().line + 1);
		return false;
	}

	try { m_Size.X() = node["Dimension"][0].as<float>(); }
	catch (YAML::Exception& e)
	{
		Error("Rect light dimension must be floats (line {})!", e.mark.line + 1);
		return false;
	}

	m_Size.Z() = node["Dimension"][1].as<float>();
	m_Area = m_Size.X() * m_Size.Z();
	m_Size /= 2.f;
	
	m_Color *= intensity;

	return true;
}

virtual void RectLight::Preprocess(const Scene& scene)
{

}
