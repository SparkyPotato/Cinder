#pragma once

#include "Core/Math/Color.h"

class Fresnel
{
public:
	virtual ~Fresnel() {}
	virtual Color Evaluate(float cosI) = 0;
};

class FresnelDielectric : public Fresnel
{
public:
	FresnelDielectric(float etaOut, float etaIn)
		: m_EtaI(etaOut), m_EtaT(etaIn)
	{}

	virtual Color Evaluate(float cosI) override
	{
		bool entering = cosI > 0.f;
		if (!entering) 
		{
			std::swap(m_EtaT, m_EtaI);
			cosI = -cosI;
		}

		float sinI = std::sqrt(std::max(0.f, 1.f - cosI * cosI));
		float sinT = m_EtaI / m_EtaT * sinI;
		if (sinT >= 1.f) { return 1.f; }

		float cosT = std::sqrt(std::max(0.f, 1.f - sinT * sinT));

		float Rparl = ((m_EtaT * cosI) - (m_EtaI * cosT)) / ((m_EtaT * cosI) + (m_EtaI * cosT));
		float Rperp = ((m_EtaI * cosI) - (m_EtaT * cosT)) / ((m_EtaI * cosI) + (m_EtaT * cosT));

		return (Rparl * Rparl + Rperp * Rperp) / 2.f;
	}

private:
	float m_EtaI, m_EtaT;
};
