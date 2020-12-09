#include "PCH.h"
#include "BxDFs/OrenNayar.h"

OrenNayar::OrenNayar(const Color& color, float roughness)
	: BxDF(BxDF::Type(BxDF::Reflection | BxDF::Diffuse)), m_Color(color)
{
	float sigma2 = roughness * roughness;
	m_A = 1.f - (sigma2 / (2.f * (sigma2 + 0.33f)));
	m_B = 0.45f * sigma2 / (sigma2 + 0.09f);
}

Color OrenNayar::Evaluate(const Vector& outgoing, const Vector& incoming) const
{
	float sinThetaI = Sin(incoming);
	float sinThetaO = Sin(outgoing);

	float cosPhi = 0;
	if (sinThetaI > 1e-4 && sinThetaO > 1e-4) 
	{
		float sinPhiI = SinPhi(incoming), cosPhiI = CosPhi(incoming);
		float sinPhiO = SinPhi(outgoing), cosPhiO = CosPhi(outgoing);
		float dCos = cosPhiI * cosPhiO + sinPhiI * sinPhiO;
		cosPhi = std::max(0.f, dCos);
	}

	float sinA, tanB;
	if (AbsCos(incoming) > AbsCos(outgoing)) 
	{
		sinA = sinThetaO;
		tanB = sinThetaI / AbsCos(incoming);
	}
	else 
	{
		sinA = sinThetaI;
		tanB = sinThetaO / AbsCos(outgoing);
	}

	return m_Color * InversePi * (m_A + m_B * cosPhi * sinA * tanB);
}
