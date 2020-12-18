#pragma once

#include "Core/Material/BxDF.h"

class LambertianBRDF : public BxDF
{
public:
	LambertianBRDF(const Color& base);

	virtual Color Evaluate(const Vector& outgoing, const Vector& incoming) const override;

private:
	Color m_Base;
};
