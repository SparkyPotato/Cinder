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

#include "PCH.h"
#include "Lights/Area.h"

AreaLight::AreaLight(uint32_t samples, const Texture* emission, float intensity, const Object* object)
	: Light(samples, Transform(), false), m_Color(emission), m_Intensity(intensity), m_Object(object)
{}

Color AreaLight::Sample(const Interaction& interaction, Sampler* sampler, Vector& incoming, float& pdf, Occlusion& tester) const
{
	Interaction i = m_Object->Sample(sampler, pdf);
	tester.Point1 = interaction;
	tester.Point2 = i;

	Vector d = i.HitPoint - interaction.HitPoint;
	if (Dot(-d, i.GNormal) < 0.f) { pdf = 0.f; return Color(); }
	incoming = d.GetNormalized();

	pdf = d.GetLengthSquare() * pdf / std::abs(Dot(i.GNormal, -incoming));

	return m_Color->Evaluate(i) * m_Intensity;
}

Color AreaLight::EvaluateAlong(const Ray& ray) const
{
	Interaction i;
	if (m_Object->Intersect(ray, i))
	{
		return m_Color->Evaluate(i) * m_Intensity;
	}

	return Color();
}

bool AreaLight::Parse(const YAML::Node& node)
{
	return false;
}
