#pragma once

#include "Core/Material/BxDF.h"

class OrenNayar : public BxDF
{
public:
	OrenNayar(const Color& color, float roughness);

	virtual Color Evaluate(const Vector& outgoing, const Vector& incoming) const override;

private:
	Color m_Color;

	float m_A, m_B;
};
