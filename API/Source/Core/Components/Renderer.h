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

#pragma once

#include "Core/Components/Registry.h"
#include "Core/Components/Framebuffer.h"
#include "Core/Scene/Scene.h"

/// Renderer responsible for converting a scene description into an image.
/// Go ham.
class Renderer
{
public:
	/// Virtual destructor.
	virtual ~Renderer() = default;

	/// Render.
	///
	/// \param scene The scene to render.
	/// \param framebuffer The framebuffer to render into.
	virtual void Render(const Scene& scene, Framebuffer& framebuffer) = 0;

	/// Parse the required parameters.
	///
	/// \param node The node containing the data.
	/// 
	/// \return If the parse succeeded.
	virtual bool Parse(const YAML::Node& node) = 0;
};

template<>
struct YAML::convert<up<Renderer>>
{
	static bool decode(const Node& node, up<Renderer>& renderer);
};

#define RENDERER(type, className) \
up<Renderer> Spawn##className() { return std::make_unique<className>(); } \
struct Register##className \
{ \
	Register##className() { Registry::Get()->GRenderers.emplace(#type, &Spawn##className); } \
}; \
static Register##className StaticRegister##className;
