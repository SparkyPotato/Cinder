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
#include "Materials/Emission.h"

#include "BxDFs/Lambertian.h"

MATERIAL(Emission, Emission)

Emission::Emission(const std::string& name)
	: Material(name)
{}

void Emission::Compute(Interaction& interaction, MemoryArena& arena) const
{
	interaction.Bsdf = arena.Allocate<BSDF>(interaction);

	
	interaction.Bsdf->Add(arena.Allocate<LambertianBRDF>(Color()));
}

const Texture* Emission::GetEmission() const
{
	return m_Color.get();
}

float Emission::GetEmissionIntensity() const
{
	return m_Intensity;
}

bool Emission::Parse(const YAML::Node& node)
{
	if (!node["Color"])
	{
		Error("Emission material does not have color (line {})!", node.Mark().line + 1);
		return false;
	}
	try { m_Color = node["Color"].as<up<Texture>>(); }
	catch (...) { return false; }

	if (!node["Intensity"])
	{
		Error("Emission material does not have an intensity (line {})!", node.Mark().line + 1);
		return false;
	}
	try { m_Intensity = node["Intensity"].as<float>(); }
	catch (YAML::Exception& e)
	{
		Error("Emission material intensity must be a float (line {})", e.mark.line + 1);
		return false;
	}

	return true;
}
