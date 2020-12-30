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

/// Base class for all materials.
class Material
{
public:
	/// Construct a material.
	///
	/// \param name The name of the material.
	Material(const std::string& name)
		: Name(name)
	{}

	/// Virtual destructor.
	virtual ~Material() = default;

	/// Calculate the shading normal of the interaction.
	///
	/// \param map The normal map to used.
	/// \param interaction The interaction to update.
    void NormalMap(const up<Texture>& map, Interaction& interaction) const;

	/// Compute the material BSDF at an interaction.
	///
	/// \param interaction The interaction to update.
	/// \param arena The arena to use to allocate the BSDF.
	virtual void Compute(Interaction& interaction, MemoryArena& arena) const = 0;

	/// Parse the required parameters.
	///
	/// \param node The node containing the data.
	/// 
	/// \return If the parse succeeded.
	virtual bool Parse(const YAML::Node& node) = 0;

	/// Get the emission of the material.
	///
	/// \return The emission texture.
	virtual const Texture* GetEmission() const { return nullptr; }

	/// Get the intensity of the emission.
	///
	/// \return The emission intensity.
	virtual float GetEmissionIntensity() const { return 0.f; }

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
