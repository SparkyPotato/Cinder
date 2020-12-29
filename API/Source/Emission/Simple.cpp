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
#include "Emission/Simple.h"

#include "Core/Scene/Object.h"

EMISSION(Simple, SimpleEmission)

SimpleEmission::SimpleEmission(uint32_t samples)
	: Emission(samples)
{}

Color SimpleEmission::Evaluate(const Interaction& interaction) const
{
	if (Dot(Point() - interaction.HitPoint, interaction.GNormal) < 0.f) { return Color(); }

	return m_Color;
}

Color SimpleEmission::Sample(const Interaction& interaction, Sampler* sampler, Vector& incoming, float& pdf, Occlusion& tester) const
{
	Interaction i = Owner->GetGeometry()->Sample(sampler, pdf);
	i.HitPoint = Owner->ToCamera(i.HitPoint);
	i.GNormal = Owner->ToCamera(i.GNormal);

	Vector d = i.HitPoint - interaction.HitPoint;
	if (Dot(-d, i.GNormal) < 0.f) { pdf = 0.f; return Color(); }
	incoming = d.GetNormalized();

	tester.Point1 = interaction;
	tester.Point2 = i;

	return m_Color / d.GetLengthSquare();
}

bool SimpleEmission::Parse(const YAML::Node& node)
{
	if (!node["Color"])
	{
		Error("Simple emission does not have color (line {})!", node.Mark().line + 1);
		return false;
	}
	if (!node["Intensity"])
	{
		Error("Simple emission does not have intensity (line {})!", node.Mark().line + 1);
		return false;
	}

	try { m_Color = node["Color"].as<Color>(); }
	catch (...) { return false; }

	try { m_Color *= node["Intensity"].as<float>(); }
	catch (YAML::Exception& e)
	{
		Error("Emission intensity must be a float (line {})!", e.mark.line + 1);
		return false;
	}

	return true;
}
