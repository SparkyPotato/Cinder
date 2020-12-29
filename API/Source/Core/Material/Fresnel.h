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
