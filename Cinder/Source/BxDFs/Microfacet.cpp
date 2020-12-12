#include "PCH.h"
#include "BxDFs/Microfacet.h"

MicrofacetReflection::MicrofacetReflection(const Color& base, Microfacet* microfacet, Fresnel* fresnel)
	: BxDF(Type(Reflection | Glossy)), m_Base(base), m_Microfacet(microfacet), m_Fresnel(fresnel)
{}

Color MicrofacetReflection::Evaluate(const Vector& outgoing, const Vector& incoming) const
{
	float cosO = AbsCos(outgoing), cosI = AbsCos(incoming);
	Normal normal = Normal(incoming + outgoing);
	if (cosI == 0.f || cosO == 0.f) { return Color(); }
	if (normal.X() == 0.f && normal.Y() == 0.f && normal.Z() == 0.f) { return Color(); }

	normal.Normalize();
	Color fresnel = m_Fresnel->Evaluate(Dot(incoming, FlipAlong(normal, Vector(0.f, 1.f, 0.f))));

	return m_Base * m_Microfacet->Evaluate(Vector(normal)) * m_Microfacet->Masking(outgoing, incoming) * 
		fresnel / (4 * cosI * cosO);
}

Color MicrofacetReflection::EvaluateSample(const Vector& outgoing, Vector& incoming, const std::pair<float, float>& sample, float& pdf) const
{
	if (outgoing.Y() == 0.f) { return Color(); }

	Vector normal = m_Microfacet->SampleNormal(outgoing, sample);
	if (Dot(outgoing, normal) < 0.f) { return Color(); }

	incoming = Reflect(outgoing, normal);
	if (!SameHemisphere(outgoing, incoming)) { return Color(); }

	pdf = m_Microfacet->Pdf(outgoing, normal) / (4.f * Dot(outgoing, normal));
	return Evaluate(outgoing, incoming);
}

float MicrofacetReflection::Pdf(const Vector& outgoing, const Vector& incoming) const
{
	if (!SameHemisphere(outgoing, incoming)) { return 0.f; }

	Vector normal = (incoming + outgoing).Normalize();
	return m_Microfacet->Pdf(outgoing, normal) / (4 * Dot(outgoing, normal));
}
