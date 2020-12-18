#include "PCH.h"
#include "BxDFs/Specular.h"

SpecularReflection::SpecularReflection(const Color& base, Fresnel* fresnel)
	: BxDF(Type(Reflection | Specular)), m_Base(base), m_Fresnel(fresnel)
{}

Color SpecularReflection::Evaluate(const Vector& outgoing, const Vector& incoming) const
{
	return Color();
}

Color SpecularReflection::EvaluateSample(const Vector& outgoing, Vector& incoming, Sampler* sampler, float& pdf) const
{
	incoming = Vector(-outgoing.X(), outgoing.Y(), -outgoing.Z());
	pdf = 1.f;
	return m_Fresnel->Evaluate(Cos(incoming)) * m_Base / AbsCos(incoming);
}

float SpecularReflection::Pdf(const Vector& outgoing, const Vector& incoming) const
{
	return 0.f;
}

SpecularTransmission::SpecularTransmission(const Color& base, float etaOut, float etaIn)
	: BxDF(BxDF::Type(BxDF::Transmission | BxDF::Specular)), m_Color(base), m_EtaIn(etaIn), m_EtaOut(etaOut),
	m_Fresnel(etaOut, etaIn)
{}

Color SpecularTransmission::Evaluate(const Vector& outgoing, const Vector& incoming) const
{
	return Color();
}

Color SpecularTransmission::EvaluateSample(const Vector& outgoing, Vector& incoming, Sampler* sampler, float& pdf) const
{
	bool entering = Cos(outgoing) > 0;
	float etaI = entering ? m_EtaOut : m_EtaIn;
	float etaT = entering ? m_EtaIn : m_EtaOut;
	
	if (!Refract(outgoing, FlipAlong(Normal(0.f, 1.f, 0.f), outgoing), etaI / etaT, incoming)) { pdf = 0.f; return Color(); }
	
	pdf = 1.f;
	Color c = m_Color * (Color(1.f) - m_Fresnel.Evaluate(Cos(incoming)));
	
	c *= (etaI * etaI) / (etaT * etaT);
	return c / AbsCos(incoming);
}

float SpecularTransmission::Pdf(const Vector& outgoing, const Vector& incoming) const
{
	return 0.f;
}
