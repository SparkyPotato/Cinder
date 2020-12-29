//    Copyright 2021 SparkyPotato
//
//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
//    
//    http://www.apache.org/licenses/LICENSE-2.0
//    
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.

#include "PCH.h"
#include "Core/Material/Fresnel.h"

FresnelDielectric::FresnelDielectric(float etaOut, float etaIn)
	: m_EtaI(etaOut), m_EtaT(etaIn)
{}

Color FresnelDielectric::Evaluate(float cosI) const
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

FresnelConductor::FresnelConductor(Color etaOut, Color etaIn, Color absorption)
	: m_EtaI(etaOut), m_EtaT(etaIn), m_K(absorption)
{}

Color Sqrt(const Color& color)
{
	return Color(std::sqrt(color.R()), std::sqrt(color.G()), std::sqrt(color.B()));
}

Color FresnelConductor::Evaluate(float cosI) const
{
	Color eta = m_EtaT / m_EtaI;
	Color etak = m_K / m_EtaI;

	float cosI2 = cosI * cosI;
	float sinI2 = 1.f - cosI2;
	Color eta2 = eta * eta;
	Color etak2 = etak * etak;

	Color t0 = eta2 - etak2 - sinI2;
	Color a2plusb2 = Sqrt(t0 * t0 + eta2 * etak2 * 4.f);
	Color t1 = a2plusb2 + cosI2;
	Color a = Sqrt((a2plusb2 + t0) * 0.5f);
	Color t2 = a * cosI * 2.f;
	Color Rs = (t1 - t2) / (t1 + t2);

	Color t3 = a2plusb2 * cosI2 + sinI2 * sinI2;
	Color t4 = t2 * sinI2;
	Color Rp = Rs * (t3 - t4) / (t3 + t4);

	return (Rp + Rs) * 0.5f;
}

Color PerfectFresnel::Evaluate(float cosI) const
{
	return Color(1.f);
}
