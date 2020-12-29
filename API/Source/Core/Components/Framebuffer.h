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
#include "Core/Math/Color.h"

class BufferTile
{
public:
	virtual ~BufferTile() = default;
	
	virtual Color GetPixel(uint32_t x, uint32_t y) = 0;
	virtual void SetPixel(const Color& color, uint32_t x, uint32_t y) = 0;
};

class Framebuffer
{
public:
	Framebuffer(uint32_t width, uint32_t height)
		: Width(width), Height(height)
	{}
	virtual ~Framebuffer() = default;

	virtual BufferTile* GetBufferTile(uint32_t xmin, uint32_t xmax, uint32_t ymin, uint32_t ymax) = 0;
	virtual void SaveBufferTile(BufferTile* tile) = 0;
	virtual bool Ouput(const std::string& file) = 0;

	virtual bool Parse(const YAML::Node& node) = 0;

	uint32_t Width;
	uint32_t Height;
};

template<>
struct YAML::convert<up<Framebuffer>>
{
	static bool decode(const Node& node, up<Framebuffer>& renderer);
};

#define FRAMEBUFFER(type, className) \
up<Framebuffer> Spawn##className(uint32_t width, uint32_t height) { return std::make_unique<className>(width, height); } \
struct Register##className \
{ \
	Register##className() { Registry::Get()->GFramebuffers.emplace(#type, &Spawn##className); } \
}; \
static Register##className StaticRegister##className;
