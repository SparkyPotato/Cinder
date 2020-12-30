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
#include "Lights/Directional.h"

#include "Core/Scene/Scene.h"

LIGHT(Directional, DirectionalLight)

DirectionalLight::DirectionalLight(uint32_t samples, const Transform& transform)
	: Light(samples, transform)
{}

Color DirectionalLight::Sample(const Interaction& interaction, Sampler* sampler, Vector& incoming, float& pdf, Occlusion& tester) const
{
	incoming = m_Incoming;
	pdf = 1.f;

	tester.Point1 = interaction;
	Interaction r;
	r.HitPoint = interaction.HitPoint + m_Incoming * m_Radius;
	tester.Point2 = r;

	return m_Color;
}

bool DirectionalLight::Parse(const YAML::Node& node)
{
	if (!node["Color"])
	{
		Error("Directional light does not have color (line {})!", node.Mark().line + 1);
		return false;
	}
	if (!node["Intensity"])
	{
		Error("Directional light does not have intensity (line {})!", node.Mark().line + 1);
		return false;
	}

	try { m_Color = node["Color"].as<Color>(); }
	catch (...) { return false; }

	try { m_Intensity = node["Intensity"].as<float>(); }
	catch (YAML::Exception& e)
	{
		Error("Directional light intensity must be a float (line {})!", e.mark.line + 1);
		return false;
	}

	m_Color *= m_Intensity;

	return true;
}

void DirectionalLight::Preprocess(const Scene& scene)
{
	m_Incoming = -ToCamera(Vector(0.f, 0.f, 1.f));
	scene.GetBound().GetBoundingSphere(m_Center, m_Radius);
	m_Radius *= 2.f;
}
