#pragma once

#include "Core/Material/BxDF.h"

class LambertianBRDF : public BxDF
{
public:
	LambertianBRDF(const Color& base);

	virtual Color Evaluate(const Vector& outgoing, const Vector& incoming) const override;

	virtual Color Reflectance(const Vector& outgoing, uint32_t sampleCount, const std::pair<float, float>* samples) const override;

private:
	Color m_Base;
};
