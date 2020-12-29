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

#include "Core/Math/Vector.h"
#include "Core/Material/BxDF.h"
#include "Core/Components/Sampler.h"

class Microfacet
{
public:
	virtual ~Microfacet() = default;

	virtual float Evaluate(const Vector& normal) const = 0;
	virtual float Lambda(const Vector& w) const = 0;
	float Masking(const Vector& w) const;
	float Masking(const Vector& outgoing, const Vector& incoming) const;
	virtual Vector SampleNormal(const Vector& outgoing, Sampler* sampler) const = 0;
	float Pdf(const Vector& outgoing, const Vector& normal) const;
	
protected:
	Microfacet(bool sampleVisibleArea);
	
	const bool m_SampleVisible;
};

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
