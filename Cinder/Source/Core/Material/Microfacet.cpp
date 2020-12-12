#include "PCH.h"
#include "Core/Material/Microfacet.h"

Microfacet::Microfacet(bool sampleVisibleArea)
	: m_SampleVisible(sampleVisibleArea)
{}

float Microfacet::Masking(const Vector& outgoing, const Vector& incoming) const
{
	return 1 / (1 + Lambda(outgoing) + Lambda(incoming));
}

float Microfacet::Masking(const Vector& w) const
{
	return 1 / (1 + Lambda(w));
}

float Microfacet::Pdf(const Vector& outgoing, const Vector& normal) const
{
	if (m_SampleVisible)
	{
		return Evaluate(normal) * Masking(outgoing) * std::abs(Dot(outgoing, normal)) / AbsCos(outgoing);
	}
	else
	{
		return Evaluate(normal) * AbsCos(normal);
	}
}

TrowbridgeReitz::TrowbridgeReitz(float alphaX, float alphaY, bool sampleVisibile)
	: Microfacet(sampleVisibile), m_AlphaX(alphaX), m_AlphaY(alphaY)
{}

float TrowbridgeReitz::Evaluate(const Vector& normal) const
{
	float tan2 = Tan2(normal);
	if (std::isinf(tan2)) { return 0.f; }

	const float cos4 = Cos2(normal) * Cos2(normal);
	float e = (Cos2Phi(normal) / (m_AlphaX * m_AlphaX) + Sin2Phi(normal) / (m_AlphaY * m_AlphaY)) * tan2;
	return 1.f / (Pi * m_AlphaX * m_AlphaY * cos4 * (1.f + e) * (1.f + e));
}

float TrowbridgeReitz::Lambda(const Vector& w) const
{
	float absTan = std::abs(Tan(w));
	if (std::isinf(absTan)) { return 0.f; }

	float alpha = std::sqrt(Cos2Phi(w) * m_AlphaX * m_AlphaX + Sin2Phi(w) * m_AlphaY * m_AlphaY);
	float alpha2Tan2 = (alpha * absTan) * (alpha * absTan);
	return (-1.f + std::sqrt(1.f + alpha2Tan2)) / 2.f;
}

float TrowbridgeReitz::RoughnessToAlpha(float roughness) 
{
	roughness = std::max(roughness, 0.001f);
	float x = std::log(roughness);

	return 1.62142f + 0.819955f * x + 0.1734f * x * x + 0.0171201f * x * x * x +
		0.000640711f * x * x * x * x;
}
