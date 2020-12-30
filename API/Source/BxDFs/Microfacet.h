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

#include "Core/Material/BxDF.h"
#include "Core/Material/MicrofacetDistribution.h"
#include "Core/Material/Fresnel.h"

/// BRDF that follows the Cook-Torrance model of perfectly specular
/// reflection over a surface with microfacets.
class MicrofacetReflection : public BxDF 
{
public:
	/// Construct the BRDF.
	///
	/// \param base The base color (albedo).
	/// \param microfacet The microfacet distribution.
	/// \param fresnel The fresnel object to control the ratio of reflected and transmitted light.
	MicrofacetReflection(const Color& base, Microfacet* microfacet, Fresnel* fresnel);

	virtual Color Evaluate(const Vector& outgoing, const Vector& incoming) const override;
	virtual Color EvaluateSample(const Vector& outgoing, Vector& incoming, Sampler* sampler, float& pdf) const override;

	virtual float Pdf(const Vector& outgoing, const Vector& incoming) const override;

private:
	const Color m_Base;
	const Microfacet* m_Microfacet;
	const Fresnel* m_Fresnel;
};

/// BTDF that follows the Cook-Torrance model of perfectly specular
/// transmission through a surface with microfacets.
class MicrofacetTransmission : public BxDF
{
public:
	/// Construct the BRDF.
	///
	/// \param base The base color (albedo).
	/// \param microfacet The microfacet distribution.
	/// \param etaOut The refractive index outside the surface.
	/// \param etaIn The refractive index inside the surface
	MicrofacetTransmission(const Color& base, Microfacet* microfacet, float etaOut, float etaIn);

	virtual Color Evaluate(const Vector& outgoing, const Vector& incoming) const override;
	virtual Color EvaluateSample(const Vector& outgoing, Vector& incoming, Sampler* sampler, float& pdf) const override;

	virtual float Pdf(const Vector& outgoing, const Vector& incoming) const override;

private:
	const Color m_Base;
	const Microfacet* m_Microfacet;
	const FresnelDielectric m_Fresnel;
	float m_EtaI, m_EtaT;
};
