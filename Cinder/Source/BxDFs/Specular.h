#pragma once

#include "Core/Material/BxDF.h"
#include "Core/Material/Fresnel.h"

class SpecularReflection : public BxDF
{
public:
	SpecularReflection(const Color& base, Fresnel* fresnel);

	virtual Color Evaluate(const Vector& outgoing, const Vector& incoming) const override;

	virtual Color EvaluateSample(const Vector& outgoing, Vector& incoming, const std::pair<float, float>& sample, float& pdf) const override;

private:
	Color m_Base;
	Fresnel* m_Fresnel;
};
