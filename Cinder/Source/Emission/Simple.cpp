#include "PCH.h"
#include "Emission/Simple.h"

#include "Core/Scene/Object.h"

EMISSION(Simple, SimpleEmission)

SimpleEmission::SimpleEmission(uint32_t samples)
	: Emission(samples)
{}

Color SimpleEmission::Evaluate(const Interaction& interaction) const
{
	if (Dot(Point() - interaction.HitPoint, interaction.GNormal) < 0.f) { return Color(); }

	return m_Color->Evaluate(interaction);
}

Color SimpleEmission::Sample(const Interaction& interaction, Sampler* sampler, Vector& incoming, float& pdf, Occlusion& tester) const
{
	Interaction i = Owner->GetGeometry()->Sample(sampler, pdf);
	i.HitPoint = Owner->ToCamera(i.HitPoint);
	i.GNormal = Owner->ToCamera(i.GNormal);

	Vector d = i.HitPoint - interaction.HitPoint;
	if (Dot(-d, i.GNormal) < 0.f) { pdf = 0.f; return Color(); }
	incoming = d.GetNormalized();

	tester.Point1 = interaction;
	tester.Point2 = i;

	return m_Color->Evaluate(i) / d.GetLengthSquare();
}

bool SimpleEmission::Parse(const YAML::Node& node)
{
	if (!node["Color"])
	{
		Error("Simple emission does not have color (line {})!", node.Mark().line + 1);
		return false;
	}
	if (!node["Intensity"])
	{
		Error("Simple emission does not have intensity (line {})!", node.Mark().line + 1);
		return false;
	}

	try { m_Color = node["Color"].as<up<Texture>>(); }
	catch (...) { return false; }

	try { m_Intensity = node["Intensity"].as<float>(); }
	catch (YAML::Exception& e)
	{
		Error("Emission intensity must be a float (line {})!", e.mark.line + 1);
		return false;
	}

	return true;
}
