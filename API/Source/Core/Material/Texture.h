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

#include "Core/Math/Color.h"
#include "Core/Scene/Interaction.h"
#include "Core/Components/Registry.h"

/// A texture.
class Texture
{
public:
	/// Virtual destructor.
	virtual ~Texture() = default;

	/// Evaluate the texture function.
	///
	/// \param interaction The interaction to evaluate the texture at.
	/// 
	/// \return The value of the texture function.
	virtual Color Evaluate(const Interaction& interaction) const = 0;
	
	/// Parse the required parameters.
	///
	/// \param node The node containing the data.
	/// 
	/// \return If the parse succeeded.
	virtual bool Parse(const YAML::Node& node) = 0;
};

template<>
struct YAML::convert<up<Texture>>
{
	static bool decode(const Node& node, up<Texture>& cubemap);
};

#define TEXTURE(type, className) \
up<Texture> Spawn##className() { return std::make_unique<className>(); } \
struct Register##className \
{ \
	Register##className() { Registry::Get()->GTextures.emplace(#type, &Spawn##className); } \
}; \
static Register##className StaticRegister##className;
