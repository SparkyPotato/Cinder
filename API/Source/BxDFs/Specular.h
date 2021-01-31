//    Copyright 2021 Shaye Garg
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

#include "Core/Material/BxDF.h"
#include "Core/Material/Fresnel.h"

/// BRDF that models perfect specular reflection.
class SpecularReflection : public BxDF
{
public:
	/// Construct the BRDF
	///
	/// \param base The base color (albedo).
	/// \param fresnel The fresnel object to control the amount of light reflected.
	SpecularReflection(const Color& base, Fresnel* fresnel);

	virtual Color Evaluate(const Vector& outgoing, const Vector& incoming) const override;
	virtual Color EvaluateSample(const Vector& outgoing, Vector& incoming, Sampler* sampler, float& pdf) const override;

	virtual float Pdf(const Vector& outgoing, const Vector& incoming) const override;

private:
	Color m_Base;
	Fresnel* m_Fresnel;
};

/// BTDF that models perfect specular transmission.
/// Takes refraction into account.
class SpecularTransmission : public BxDF
{
public:
	/// Construct the BTDF
	///
	/// \param base The base color (albedo).
	/// \param etaOut The refractive index outside the surface.
	/// \param etaIn The refractive index inside the surface.
	SpecularTransmission(const Color& base, float etaOut, float etaIn);
	
	virtual Color Evaluate(const Vector& outgoing, const Vector& incoming) const override;
	virtual Color EvaluateSample(const Vector& outgoing, Vector& incoming, Sampler* sampler, float& pdf) const override;

	virtual float Pdf(const Vector& outgoing, const Vector& incoming) const override;
	
private:
	Color m_Color;
	float m_EtaIn, m_EtaOut;
	FresnelDielectric m_Fresnel;
};
