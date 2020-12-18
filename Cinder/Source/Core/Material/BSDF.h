#pragma once

#include "Core/Scene/Interaction.h"
#include "Core/Material/BxDF.h"
#include "Core/Components/Sampler.h"

class BSDF
{
public:
	BSDF(const Interaction& interaction);

	void Add(BxDF* bxdf);
	uint16_t Components(BxDF::Type type = BxDF::All) const;

	Vector ToLocal(const Vector& vector) const;
	Vector ToWorld(const Vector& vector) const;

	Color Evaluate(const Vector& outgoing, const Vector& incoming, BxDF::Type type = BxDF::All) const;
	Color EvaluateSample(const Vector& outgoing, Vector& incoming, Sampler* sampler, float& pdf, BxDF::Type type = BxDF::All, BxDF::Type* sampled = nullptr) const;

	virtual float Pdf(const Vector& outgoing, const Vector& incoming, BxDF::Type type = BxDF::All) const;
private:
	Vector m_GNormal, m_X, m_Z;
	Vector m_SNormal;

	static constexpr uint16_t m_Max = 8;
	uint16_t m_BxDFCount = 0;
	BxDF* m_BxDFs[m_Max];
};
