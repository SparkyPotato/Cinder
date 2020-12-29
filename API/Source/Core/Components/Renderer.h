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

#include "Core/Components/Registry.h"
#include "Core/Components/Framebuffer.h"
#include "Core/Scene/Scene.h"

class Renderer
{
public:
	virtual ~Renderer() = default;

	virtual void Render(const Scene& scene, Framebuffer& framebuffer) = 0;
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
