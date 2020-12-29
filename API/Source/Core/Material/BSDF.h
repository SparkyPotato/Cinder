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

class BSDF
{
public:
	BSDF(const Interaction& interaction);

	void Add(BxDF* bxdf);
	uint16_t Components(BxDF::Type type = BxDF::All) const;

	Vector ToLocal(const Vector& vector) const;
	Vector ToWorld(const Vector& vector) const;

	Color Evaluate(const Vector& outgoing, const Vector& incoming, BxDF::Type type = BxDF::All) const;
	Color EvaluateSample(const Vector& outgoing, Vector& incoming, Sampler* sampler, float& pdf, BxDF::Type type = BxDF::All, BxDF::Type* sampled = nullptr) const;

	virtual float Pdf(const Vector& outgoing, const Vector& incoming, BxDF::Type type = BxDF::All) const;
private:
	Vector m_GNormal, m_X, m_Z;
	Vector m_SNormal;

	static constexpr uint16_t m_Max = 8;
	uint16_t m_BxDFCount = 0;
	BxDF* m_BxDFs[m_Max];
};
