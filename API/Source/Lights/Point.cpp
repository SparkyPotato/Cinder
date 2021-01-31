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
#include "Lights/Point.h"

LIGHT(Point, PointLight)

PointLight::PointLight(uint32_t samples, const Transform& transform)
	: Light(samples, transform, true)
{}

Color PointLight::Sample(const Interaction& interaction, Sampler* sampler, Vector& incoming, float& pdf, Occlusion& tester) const
{
	Vector d = m_Position - interaction.HitPoint;
	incoming = d.GetNormalized();
	pdf = 1.f;

	tester.Point1 = interaction;
	tester.Point2.HitPoint = m_Position;

	return m_Color / d.GetLengthSquare();
}

bool PointLight::Parse(const YAML::Node& node)
{
	if (!node["Color"])
	{
		Error("Point light does not have color (line {})!", node.Mark().line + 1);
		return false;
	}
	if (!node["Intensity"])
	{
		Error("Point light does not have intensity (line {})!", node.Mark().line + 1);
		return false;
	}

	try { m_Color = node["Color"].as<Color>(); }
	catch (...) { return false; }

	float intensity;
	try { intensity = node["Intensity"].as<float>(); }
	catch (YAML::Exception& e)
	{
		Error("Point light intensity must be a float (line {})!", e.mark.line + 1);
		return false;
	}

	m_Color *= intensity;

	return true;
}

void PointLight::Preprocess(const Scene& scene)
{
	m_Position = ToCamera(Point());
}
