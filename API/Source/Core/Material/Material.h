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

#include "Core/Material/BSDF.h"
#include "Core/Material/Texture.h"
#include "Core/Components/Registry.h"

class Material
{
public:
	Material(const std::string& name)
		: Name(name)
	{}
	virtual ~Material() = default;

    void NormalMap(const up<Texture>& map, Interaction& interaction) const;

	virtual void Compute(Interaction& interaction, MemoryArena& arena) const = 0;
	virtual bool Parse(const YAML::Node& node) = 0;

	std::string Name;
};

template<>
struct YAML::convert<up<Material>>
{
	static bool decode(const Node& node, up<Material>& material);
};

#define MATERIAL(type, className) \
up<Material> Spawn##className(const std::string& name) { return std::make_unique<className>(name); } \
struct Register##className \
{ \
Register##className() { Registry::Get()->GMaterials.emplace(#type, &Spawn##className); } \
}; \
static Register##className StaticRegister##className;
