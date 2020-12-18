#include "PCH.h"
#include "Core/Material/MicrofacetDistribution.h"

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

void TrowbridgeReitzSample11(float cosTheta, float U1, float U2, float& slopeX, float& slopeZ)
{
	if (cosTheta > .9999f) {
		float r = sqrt(U1 / (1 - U1));
		float phi = 6.28318530718f * U2;
		slopeX = r * cos(phi);
		slopeZ = r * sin(phi);
		return;
	}

	float sinTheta =
		std::sqrt(std::max(0.f, 1.f - cosTheta * cosTheta));
	float tanTheta = sinTheta / cosTheta;
	float a = 1.f / tanTheta;
	float G1 = 2.f / (1.f + std::sqrt(1.f + 1.f / (a * a)));

	float A = 2.f * U1 / G1 - 1.f;
	float tmp = 1.f / (A * A - 1.f);
	if (tmp > 1e10f) tmp = 1e10f;
	float B = tanTheta;
	float D = std::sqrt(std::max(B * B * tmp * tmp - (A * A - B * B) * tmp, 0.f));
	float slopeX1 = B * tmp - D;
	float slopeX2 = B * tmp + D;
	slopeX = (A < 0 || slopeX2 > 1.f / tanTheta) ? slopeX1 : slopeX2;

	float S;
	if (U2 > 0.5f) {
		S = 1.f;
		U2 = 2.f * (U2 - .5f);
	}
	else {
		S = -1.f;
		U2 = 2.f * (.5f - U2);
	}
	float z = (U2 * (U2 * (U2 * 0.27385f - 0.73369f) + 0.46341f)) /
		(U2 * (U2 * (U2 * 0.093073f + 0.309420f) - 1.000000f) + 0.597999f);
	slopeZ = S * z * std::sqrt(1.f + slopeX * slopeX);
}

Vector TrowbridgeReitzSample(const Vector& incoming, float alphaX, float alphaY, float U1, float U2) 
{
	Vector inStretched = Vector(alphaX * incoming.X(), alphaY * incoming.Y(), incoming.Z()).Normalize();

	float slopeX, slopeZ;
	TrowbridgeReitzSample11(Cos(inStretched), U1, U2, slopeX, slopeZ);

	float tmp = CosPhi(inStretched) * slopeX - SinPhi(inStretched) * slopeZ;
	slopeZ = SinPhi(inStretched) * slopeX + CosPhi(inStretched) * slopeZ;
	slopeX = tmp;

	slopeX = alphaX * slopeX;
	slopeZ = alphaY * slopeZ;

	return Vector(-slopeX, 1.f, -slopeZ).Normalize();
}

Vector TrowbridgeReitz::SampleNormal(const Vector& outgoing, Sampler* sampler) const
{
	std::pair<float, float> sample = sampler->Get2D();

	Vector normal;
	if (!m_SampleVisible) 
	{
		float cos = 0, phi = (2 * Pi) * sample.first;
		if (m_AlphaX == m_AlphaY)
		{
			float tan2 = m_AlphaX * m_AlphaX * sample.first / (1.0f - sample.first);
			cos = 1.f / std::sqrt(1.f + tan2);
		}
		else 
		{
			phi = std::atan(m_AlphaY / m_AlphaX * std::tan(2.f * Pi * sample.second + .5f * Pi));
			if (sample.second > .5f) { phi += Pi; }
			float sinPhi = std::sin(phi), cosPhi = std::cos(phi);
			const float alphaX2 = m_AlphaX * m_AlphaX, alphay2 = m_AlphaY * m_AlphaY;
			const float alpha2 = 1.f / (cosPhi * cosPhi / alphaX2 + sinPhi * sinPhi / alphay2);
			float tan2 = alpha2 * sample.first / (1.f - sample.first);
			cos = 1 / std::sqrt(1 + tan2);
		}
		float sin = std::sqrt(std::max(0.f, 1.f - cos * cos));
		normal = SphericalDirection(sin, cos, phi);
		if (!SameHemisphere(outgoing, normal)) normal = -normal;
	}
	else
	{
		bool flip = outgoing.Y() < 0;
		normal = TrowbridgeReitzSample(flip ? -outgoing : outgoing, m_AlphaX, m_AlphaY, sample.first, sample.second);
		if (flip) { normal = -normal; }
	}
	return normal;
}
