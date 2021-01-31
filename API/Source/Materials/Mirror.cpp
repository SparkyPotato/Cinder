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
#include "Materials/Mirror.h"

#include "Core/Material/Fresnel.h"
#include "BxDFs/Specular.h"

MATERIAL(Mirror, Mirror)

Mirror::Mirror(const std::string& name)
	: Material(name)
{}

void Mirror::Compute(Interaction& interaction, MemoryArena& arena) const
{
    NormalMap(m_Normal, interaction);
	interaction.Bsdf = arena.Allocate<BSDF>(interaction);

	auto fresnel = arena.Allocate<PerfectFresnel>();
	Color c = m_Color->Evaluate(interaction);
	interaction.Bsdf->Add(arena.Allocate<SpecularReflection>(c, fresnel));
}

bool Mirror::Parse(const YAML::Node& node)
{
	if (!node["Color"])
	{
		Error("Mirror material does not have color (line {})!", node.Mark().line + 1);
		return false;
	}
	try { m_Color = node["Color"].as<up<Texture>>(); }
	catch (...) { return false; }

    if (!node["Normal"])
	{
		Error("Glass material does not have a normal map (line {})!", node.Mark().line + 1);
		return false;
	}
	try { m_Normal = node["Normal"].as<up<Texture>>(); }
	catch (...) { return false; }

	return true;
}
