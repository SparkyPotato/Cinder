#pragma once

#include "Core/Math/RayIntersection.h"
#include "Core/Material/BxDF.h"

class BSDF
{
public:
	BSDF(const RayIntersection& intersection);

	void Add(BxDF* bxdf);
	uint16_t Components(BxDF::Type type = BxDF::All) const;

	Vector ToLocal(const Vector& vector) const;
	Vector ToWorld(const Vector& vector) const;

	virtual Color Evaluate(const Vector& outgoing, const Vector& incoming, BxDF::Type type = BxDF::All) const = 0;
	virtual Color EvaluateSample(const Vector& outgoing, Vector& incoming, const std::pair<float, float>& sample, float& pdf, BxDF::Type type = BxDF::All) const;

	virtual Color Reflectance(const Vector& outgoing, uint32_t sampleCount, const std::pair<float, float>* samples, BxDF::Type type = BxDF::All) const = 0;

	virtual float Pdf(const Vector& outgoing, const Vector& incoming, BxDF::Type type = BxDF::All) const;
private:
	Vector m_Normal, m_X, m_Y;

	static constexpr uint16_t m_Max = 8;
	uint16_t m_BxDFCount = 0;
	BxDF* m_BxDFs[m_Max];
};
