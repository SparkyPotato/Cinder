#pragma once

#include "Core/Material/BxDF.h"
#include "Core/Material/Fresnel.h"

class SpecularReflection : public BxDF
{
public:
	SpecularReflection(const Color& base, Fresnel* fresnel);

	virtual Color Evaluate(const Vector& outgoing, const Vector& incoming) const override;
	virtual Color EvaluateSample(const Vector& outgoing, Vector& incoming, const std::pair<float, float>& sample, float& pdf) const override;

	virtual float Pdf(const Vector& outgoing, const Vector& incoming) const override;

private:
	Color m_Base;
	Fresnel* m_Fresnel;
};

class SpecularTransmission : public BxDF
{
public:
	SpecularTransmission(const Color& base, float etaOut, float etaIn);
	
	virtual Color Evaluate(const Vector& outgoing, const Vector& incoming) const override;
	virtual Color EvaluateSample(const Vector& outgoing, Vector& incoming, const std::pair<float, float>& sample, float& pdf) const override;

	virtual float Pdf(const Vector& outgoing, const Vector& incoming) const override;
	
private:
	Color m_Color;
	float m_EtaIn, m_EtaOut;
	FresnelDielectric m_Fresnel;
};
