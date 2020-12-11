#include "PCH.h"
#include "BxDFs/Specular.h"

SpecularReflection::SpecularReflection(const Color& base, Fresnel* fresnel)
	: BxDF(BxDF::Type(BxDF::Reflection | BxDF::Specular)), m_Base(base), m_Fresnel(fresnel)
{}

Color SpecularReflection::Evaluate(const Vector& outgoing, const Vector& incoming) const
{
	return Color();
}

Color SpecularReflection::EvaluateSample(const Vector& outgoing, Vector& incoming, const std::pair<float, float>& sample, float& pdf) const
{
	incoming = Vector(-outgoing.X(), outgoing.Y(), -outgoing.Z());
	pdf = 1.f;
	return m_Fresnel->Evaluate(Cos(incoming)) * m_Base / AbsCos(incoming);
}
