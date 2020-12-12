#pragma once

#include "Core/Math/Vector.h"
#include "Core/Material/BxDF.h"

class Microfacet
{
public:
	virtual ~Microfacet() = default;

	virtual float Evaluate(const Vector& normal) const = 0;
	virtual float Lambda(const Vector& w) const = 0;
	float Masking(const Vector& w) const;
	float Masking(const Vector& outgoing, const Vector& incoming) const;
	virtual Vector SampleNormal(const Vector& outgoing, const std::pair<float, float>& sample) const = 0;
	float Pdf(const Vector& outgoing, const Vector& normal) const;
	
protected:
	Microfacet(bool sampleVisibleArea);
	
	const bool m_SampleVisible;
};

class TrowbridgeReitz : public Microfacet
{
public:
	TrowbridgeReitz(float alphaX, float alphaY, bool sampleVisibile = true);

	virtual float Evaluate(const Vector& normal) const override;

	virtual Vector SampleNormal(const Vector& outgoing, const std::pair<float, float>& sample) const override;

	static float RoughnessToAlpha(float roughness);

private:
	virtual float Lambda(const Vector& w) const override;

	float m_AlphaX, m_AlphaY;
};
