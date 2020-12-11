#pragma once

#include "Core/Math/Color.h"

class Fresnel
{
public:
	virtual ~Fresnel() {}
	virtual Color Evaluate(float cosI) const = 0;
};

class FresnelDielectric : public Fresnel
{
public:
	FresnelDielectric(float etaOut, float etaIn)
		: m_EtaI(etaOut), m_EtaT(etaIn)
	{}

	virtual Color Evaluate(float cosI) const override
	{
		bool entering = cosI > 0.f;
		float etaI = m_EtaI, etaT = m_EtaT;
		if (!entering) 
		{
			std::swap(etaI, etaT);
			cosI *= -1;
		}

		float sinI = std::sqrt(std::max(0.f, 1.f - cosI * cosI));
		float sinT = etaI / etaT * sinI;
		if (sinT >= 1.f) { return 1.f; }

		float cosT = std::sqrt(std::max(0.f, 1.f - sinT * sinT));

		float Rparl = ((etaT * cosI) - (etaI * cosT)) / ((etaT * cosI) + (etaI * cosT));
		float Rperp = ((etaI * cosI) - (etaT * cosT)) / ((etaI * cosI) + (etaT * cosT));

		return (Rparl * Rparl + Rperp * Rperp) / 2.f;
	}

private:
	float m_EtaI, m_EtaT;
};

class PerfectFresnel : public Fresnel
{
public:
	virtual Color Evaluate(float cosI) const override
	{
		return Color(1.f);
	}
};
