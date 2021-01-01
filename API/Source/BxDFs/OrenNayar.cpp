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
#include "BxDFs/OrenNayar.h"

OrenNayar::OrenNayar(const Color& color, float roughness)
	: BxDF(Type(Reflection | Diffuse)), m_Color(color)
{
	float sigma2 = roughness * roughness;
	m_A = 1.f - (sigma2 / (2.f * (sigma2 + 0.33f)));
	m_B = 0.45f * sigma2 / (sigma2 + 0.09f);
}

Color OrenNayar::Evaluate(const Vector& outgoing, const Vector& incoming) const
{
	float sinI = Sin(incoming);
	float sinO = Sin(outgoing);

	float cosPhi = 0;
	if (sinI > 1e-4 && sinO > 1e-4) 
	{
		float sinPhiI = SinPhi(incoming), cosPhiI = CosPhi(incoming);
		float sinPhiO = SinPhi(outgoing), cosPhiO = CosPhi(outgoing);
		float dCos = cosPhiI * cosPhiO + sinPhiI * sinPhiO;
		cosPhi = std::max(0.f, dCos);
	}

	float sinA, tanB;
	if (AbsCos(incoming) > AbsCos(outgoing)) 
	{
		sinA = sinO;
		tanB = sinI / AbsCos(incoming);
	}
	else 
	{
		sinA = sinI;
		tanB = sinO / AbsCos(outgoing);
	}

	return m_Color * InversePi * (m_A + m_B * cosPhi * sinA * tanB);
}
