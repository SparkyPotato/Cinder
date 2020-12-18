#pragma once

#include "Core/Material/BxDF.h"
#include "Core/Material/MicrofacetDistribution.h"
#include "Core/Material/Fresnel.h"

class MicrofacetReflection : public BxDF 
{
public:
	MicrofacetReflection(const Color& base, Microfacet* microfacet, Fresnel* fresnel);

	virtual Color Evaluate(const Vector& outgoing, const Vector& incoming) const override;
	virtual Color EvaluateSample(const Vector& outgoing, Vector& incoming, Sampler* sampler, float& pdf) const override;

	virtual float Pdf(const Vector& outgoing, const Vector& incoming) const override;

private:
	const Color m_Base;
	const Microfacet* m_Microfacet;
	const Fresnel* m_Fresnel;
};
