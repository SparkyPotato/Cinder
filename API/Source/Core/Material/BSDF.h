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

#include "Core/Scene/Interaction.h"
#include "Core/Material/BxDF.h"
#include "Core/Components/Sampler.h"

/// Scattering distribution (collection of BxDFs).
class BSDF
{
public:
	/// Create the BSDF.
	///
	/// \param interaction The surface interaction where the BSDF is spawned.
	BSDF(const Interaction& interaction);

	/// Add a BxDF to the BSDF.
	///
	/// \param bxdf The bxdf to add. It is NOT copied.
	void Add(BxDF* bxdf);

	/// Get the components.
	///
	/// \param type Type to match in the count.
	/// 
	/// \return The number of componenets.
	uint16_t Components(BxDF::Type type = BxDF::All) const;

	/// Convert to local shading coordinates.
	///
	/// \param vector Vector to convert.
	/// 
	/// \return The vector in local coordinates.
	Vector ToLocal(const Vector& vector) const;

	/// Convert to world (camera) coordinates.
	///
	/// \param vector Vector to convert.
	/// 
	/// \return The vector in camera coordinates.
	Vector ToWorld(const Vector& vector) const;

	/// Evaluate the reflected amount of energy.
	///
	/// \param outgoing The unit vector pointing towards the camera (view direction / wo).
	/// \param incoming The unit vector pointing towards the light (energy direction / wi).
	/// \param type The type of BxDFs to take into consideration.
	/// 
	/// \return The amount of incident energy reflected along the outgoing vector.
	Color Evaluate(const Vector& outgoing, const Vector& incoming, BxDF::Type type = BxDF::All) const;

	/// Sample the BSDF.
	///
	/// \param outgoing The unit vector pointing towards the camera (view direction / wo).
	/// \param incoming The unit vector pointing towards the light (energy direction / wi) (Is filled in by the function).
	/// \param sampler The sampler to extract samples from.
	/// \param pdf The probability density of the distribution (Is filled in by the function).
	/// \param type The type of BxDF to sample.
	/// \param sampled The type of BxDF that was sampled (Is filled in by the function).
	/// 
	/// \return The amount of incident energy reflected along the outgoing vector.
	Color Sample(const Vector& outgoing, Vector& incoming, Sampler* sampler, float& pdf, BxDF::Type type = BxDF::All, BxDF::Type* sampled = nullptr) const;

	/// Get the probability density for a pair of directions.
	///
	/// \param outgoing The unit vector pointing towards the camera (view direction / wo).
	/// \param incoming The unit vector pointing towards the light (energy direction / wi).
	/// \param type The type of BxDFs to take into consideration.
	/// 
	/// \return The probability density of the BSDF.
	virtual float Pdf(const Vector& outgoing, const Vector& incoming, BxDF::Type type = BxDF::All) const;

private:
	Vector m_GNormal, m_X, m_Z;
	Vector m_SNormal;

	static constexpr uint16_t m_Max = 8;
	uint16_t m_BxDFCount = 0;
	BxDF* m_BxDFs[m_Max];
};
