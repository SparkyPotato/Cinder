//    Copyright 2021 SparkyPotato
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
#include "BxDFs/Microfacet.h"

MicrofacetReflection::MicrofacetReflection(const Color& base, Microfacet* microfacet, Fresnel* fresnel)
	: BxDF(Type(Reflection | Glossy)), m_Base(base), m_Microfacet(microfacet), m_Fresnel(fresnel)
{}

Color MicrofacetReflection::Evaluate(const Vector& outgoing, const Vector& incoming) const
{
	float cosO = AbsCos(outgoing), cosI = AbsCos(incoming);
	Normal normal = Normal(incoming + outgoing); // Since reflection is perfectly specular across microfacet.
	if (cosI == 0.f || cosO == 0.f) { return Color(); }
	if (normal.X() == 0.f && normal.Y() == 0.f && normal.Z() == 0.f) { return Color(); }

	normal.Normalize();
	Color fresnel = m_Fresnel->Evaluate(Dot(incoming, FlipAlong(normal, Vector(0.f, 1.f, 0.f))));

	return m_Base * m_Microfacet->Evaluate(Vector(normal)) * m_Microfacet->Masking(outgoing, incoming) 
		* fresnel / (4 * cosI * cosO);
}

Color MicrofacetReflection::EvaluateSample(const Vector& outgoing, Vector& incoming, Sampler* sampler, float& pdf) const
{
	if (outgoing.Y() == 0.f) { return Color(); }

	Vector normal = m_Microfacet->SampleNormal(outgoing, sampler);
	if (Dot(outgoing, normal) < 0.f) { return Color(); }

	incoming = Reflect(outgoing, normal); // Specular reflect across sampled normal, because Cook-Torrance
	if (!SameHemisphere(outgoing, incoming)) { return Color(); } // No transmission

	pdf = m_Microfacet->Pdf(outgoing, normal) / (4.f * Dot(outgoing, normal));
	return Evaluate(outgoing, incoming);
}

float MicrofacetReflection::Pdf(const Vector& outgoing, const Vector& incoming) const
{
	if (!SameHemisphere(outgoing, incoming)) { return 0.f; }

	Vector normal = (incoming + outgoing).Normalize();
	return m_Microfacet->Pdf(outgoing, normal) / (4 * Dot(outgoing, normal));
}

MicrofacetTransmission::MicrofacetTransmission(const Color& base, Microfacet* microfacet, float etaOut, float etaIn)
	: BxDF(Type(Transmission | Glossy)), m_Base(base), m_Microfacet(microfacet), m_Fresnel(etaOut, etaIn),
	m_EtaI(etaOut), m_EtaT(etaIn)
{}

Color MicrofacetTransmission::Evaluate(const Vector& outgoing, const Vector& incoming) const
{
	if (SameHemisphere(outgoing, incoming)) { return Color(); }

	float cosO = Cos(outgoing);
	float cosI = Cos(incoming);
	if (cosI == 0.f || cosO == 0.f) { return Color(); }

	float eta = cosO > 0.f ? (m_EtaT / m_EtaI) : (m_EtaI / m_EtaT);
	Vector normal = (outgoing + incoming * eta).Normalize();
	if (normal.Y() < 0.f) { normal = -normal; }

	if (Dot(outgoing, normal) * Dot(incoming, normal) > 0.f) { return Color(); }

	Color c = m_Fresnel.Evaluate(Dot(outgoing, normal));

	float sqrtDenom = Dot(outgoing, normal) + eta * Dot(incoming, normal);
	float factor = 1.f / eta;

	return (Color(1.f) - c) * m_Base
		* std::abs(m_Microfacet->Evaluate(normal) * m_Microfacet->Masking(outgoing, incoming) * eta * eta
		* std::abs(Dot(incoming, normal)) * std::abs(Dot(outgoing, normal)) * factor * factor
		/ (cosI * cosO * sqrtDenom * sqrtDenom));
}

Color MicrofacetTransmission::EvaluateSample(const Vector& outgoing, Vector& incoming, Sampler* sampler, float& pdf) const
{
	if (outgoing.Y() == 0.f) { return Color(); }
	Vector normal = m_Microfacet->SampleNormal(outgoing, sampler);
	if (Dot(outgoing, normal) < 0.f) { return Color(); }

	float eta = Cos(outgoing) > 0.f ? (m_EtaT / m_EtaI) : (m_EtaI / m_EtaT);
	if (!Refract(outgoing, Normal(normal), eta, incoming)) { return Color(); }
	pdf = Pdf(outgoing, incoming);
	return Evaluate(outgoing, incoming);
}

float MicrofacetTransmission::Pdf(const Vector& outgoing, const Vector& incoming) const
{
	if (SameHemisphere(outgoing, incoming)) { return 0.f; }
	float eta = Cos(outgoing) > 0.f ? (m_EtaT / m_EtaI) : (m_EtaI / m_EtaT);
	Vector normal = (outgoing + incoming * eta).Normalize();

	if (Dot(outgoing, normal) * Dot(incoming, normal) > 0.f) { return 0.f; }

	float sqrtDenom = Dot(outgoing, normal) + eta * Dot(incoming, normal);
	float dNorm = std::abs((eta * eta * Dot(incoming, normal)) / (sqrtDenom * sqrtDenom));
	return m_Microfacet->Pdf(outgoing, normal) * dNorm;
}
