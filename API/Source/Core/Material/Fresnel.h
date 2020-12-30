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

/// Base class for the Fresnel functions,
/// used to determine the ratio between transmitted and reflected light for a material.
class Fresnel
{
public:
	/// Virtual destructor.
	virtual ~Fresnel() = default;

	/// Evaluate the Fresnel function.
	///
	/// \param cosI The cosine of the angle between the incident ray and the normal.
	/// 
	/// \return The amount of energy reflected.
	virtual Color Evaluate(float cosI) const = 0;
};

/// Fresnel function for dielectric substances.
class FresnelDielectric : public Fresnel
{
public:
	/// Construct the dielectric Fresnel.
	///
	/// \param etaOut The refractive index of the substance containing the incident ray.
	/// \param etaIn The refractive index of the substance inside the object.
	FresnelDielectric(float etaOut, float etaIn);

	virtual Color Evaluate(float cosI) const override;

private:
	float m_EtaI, m_EtaT;
};

/// Fresnel function for conductors.
class FresnelConductor : public Fresnel
{
public:
	/// Construct the conductor Fresnel.
	///
	/// \param etaOut The refractive index of the substance containing the incident ray.
	/// \param etaIn The refractive index of the substance inside the object.
	/// \param absorption The amount of energy absorbed by the object. 
	FresnelConductor(Color etaOut, Color etaIn, Color absorption);

	virtual Color Evaluate(float cosI) const override;

private:
	Color m_EtaI, m_EtaT, m_K;
};

/// Fresnel for perfect reflection.
class PerfectFresnel : public Fresnel
{
public:
	virtual Color Evaluate(float cosI) const override;
};
