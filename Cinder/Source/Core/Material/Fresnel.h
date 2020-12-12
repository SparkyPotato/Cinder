#pragma once

#include "Core/Math/Color.h"

class Fresnel
{
public:
	virtual ~Fresnel() = default;
	virtual Color Evaluate(float cosI) const = 0;
};

class FresnelDielectric : public Fresnel
{
public:
	FresnelDielectric(float etaOut, float etaIn);

	virtual Color Evaluate(float cosI) const override;

private:
	float m_EtaI, m_EtaT;
};

class FresnelConductor : public Fresnel
{
public:
	FresnelConductor(Color etaOut, Color etaIn, Color absorption);

	virtual Color Evaluate(float cosI) const override;

private:
	Color m_EtaI, m_EtaT, m_K;
};

class PerfectFresnel : public Fresnel
{
public:
	virtual Color Evaluate(float cosI) const override;
};
