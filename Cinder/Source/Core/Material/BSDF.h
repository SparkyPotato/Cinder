#pragma once

#include "Core/Math/RayIntersection.h"
#include "Core/Material/BxDF.h"

class BSDF
{
public:
	BSDF(const RayIntersection& intersection, const Normal& shadingNormal);

	void Add(BxDF* bxdf, float weight);
	uint16_t Components(BxDF::Type type = BxDF::All) const;

	Vector ToLocal(const Vector& vector) const;
	Vector ToWorld(const Vector& vector) const;

	Color Evaluate(const Vector& outgoing, const Vector& incoming, BxDF::Type type = BxDF::All) const;
	Color EvaluateSample(const Vector& outgoing, Vector& incoming, const std::pair<float, float>& sample,
		float& pdf, BxDF::Type* sampled = nullptr, BxDF::Type type = BxDF::All) const;

	Color Reflectance(const Vector& outgoing, uint32_t sampleCount, const std::pair<float, float>* samples, BxDF::Type type = BxDF::All) const;

	virtual float Pdf(const Vector& outgoing, const Vector& incoming, BxDF::Type type = BxDF::All) const;
private:
	Vector m_GNormal, m_X, m_Z;
	Vector m_SNormal;

	static constexpr uint16_t m_Max = 8;
	uint16_t m_BxDFCount = 0;
	float m_TotalWeight = 0.f;
	std::pair<BxDF*, float> m_BxDFs[m_Max];
};
