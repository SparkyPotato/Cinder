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

/// Tile of the framebuffer, used for tile-based rendering.
class BufferTile
{
public:
	/// Virtual destructor.
	virtual ~BufferTile() = default;
	
	/// Get the color of the pixel.
	///
	/// \param x The x-value of the pixel relative to the top-left corner of the framebuffer.
	/// \param y The y-value of the pixel relative to the top-left corner of the framebuffer.
	/// 
	/// \return The color of the requested pixel.
	virtual Color GetPixel(uint32_t x, uint32_t y) = 0;

	/// Set the color of the pixel.
	///
	/// \param color The color to set the pixel to.
	/// \param x The x-value of the pixel relative to the top-left corner of the framebuffer.
	/// \param y The y-value of the pixel relative to the top-left corner of the framebuffer.
	virtual void SetPixel(const Color& color, uint32_t x, uint32_t y) = 0;
};

/// Base class of framebuffers for storing the image being rendered.
class Framebuffer
{
public:
	/// Construct the framebuffer.
	///
	/// \param width Width of the framebuffer.
	/// \param height Height of the framebuffer.
	Framebuffer(uint32_t width, uint32_t height)
		: Width(width), Height(height)
	{}

	/// Virtual destructor.
	virtual ~Framebuffer() = default;

	/// Get a tile of the framebuffer.
	///
	/// \param xmin The minimum x-value of the tile's range (inclusive).
	/// \param xmax The maximum x-value of the tile's range (exclusive).
	/// \param ymin The minimum y-value of the tile's range (inclusive).
	/// \param ymax The maximum y-value of the tile's range (exclusive).
	/// 
	/// \return The created buffer tile.
	virtual BufferTile* GetBufferTile(uint32_t xmin, uint32_t xmax, uint32_t ymin, uint32_t ymax) = 0;


	/// Save a buffer tile to the framebuffer.
	///
	/// \param tile The tile to save. Is destroyed by the function.
	virtual void SaveBufferTile(BufferTile* tile) = 0;

	/// Output the framebuffer into a file.
	///
	/// \param file The name and path of the file to output into.
	/// The filename extension is NOT checked.
	/// 
	/// \return If the output succeeded.
	virtual bool Output(const std::string& file) = 0;

	/// Parse the required parameters.
	///
	/// \param node The node containing the data.
	/// 
	/// \return If the parse succeeded.
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
