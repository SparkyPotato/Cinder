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

#include "Core/Math/Vector.h"
#include "Core/Material/BxDF.h"
#include "Core/Components/Sampler.h"

/// Base class for distribution of microfacets on surfaces.
class Microfacet
{
public:
	/// Virtual destructor.
	virtual ~Microfacet() = default;

	/// Calculate the differential area of the microfacets.
	///
	/// \param normal The surface normal.
	/// 
	/// \return The differential area.
	virtual float Evaluate(const Vector& normal) const = 0;

	/// Calculate the shadow-masking function.
	/// 
	/// \param w The view direction.
	/// 
	/// \return Ratio of invisible masked microfacet area and visible microfacet area.
	virtual float Lambda(const Vector& w) const = 0;

	/// Calculate the masking function.
	/// 
	/// \param w The view direction.
	/// 
	/// \return Fraction of visible microfacet area.
	float Masking(const Vector& w) const;

	/// Calculate the masking function.
	/// 
	/// \param outgoing The view direction.
	/// \param incoming The light direction.
	/// 
	/// \return Fraction of visible microfacet area.
	float Masking(const Vector& outgoing, const Vector& incoming) const;

	/// Sample the microfacet normal.
	///
	/// \param outgoing The view direction.
	/// \param sampler The Sampler to use for samples.
	/// 
	/// \return The normal.
	virtual Vector SampleNormal(const Vector& outgoing, Sampler* sampler) const = 0;

	/// Calculate the probability density with an outgoing vector and a normal.
	///
	/// \param outgoing The view direction.
	/// \param normal The normal.
	/// 
	/// \return The probability density function.
	float Pdf(const Vector& outgoing, const Vector& normal) const;
	
protected:
	Microfacet(bool sampleVisibleArea);
	
	const bool m_SampleVisible;
};

/// Trowbridge-Reitz microfacet distribution.
class TrowbridgeReitz : public Microfacet
{
public:
	TrowbridgeReitz(float alphaX, float alphaY, bool sampleVisibile = true);

	virtual float Evaluate(const Vector& normal) const override;

	virtual Vector SampleNormal(const Vector& outgoing, Sampler* sampler) const override;

	static float RoughnessToAlpha(float roughness);

private:
	virtual float Lambda(const Vector& w) const override;

	float m_AlphaX, m_AlphaY;
};
