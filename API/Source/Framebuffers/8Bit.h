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

#include "Core/Components/Framebuffer.h"

struct Pixel
{
	uint8_t R, G, B;
};

class EightBitTile : public BufferTile
{
public:
	EightBitTile(Pixel* data, uint32_t xmin, uint32_t xmax, uint32_t ymin, uint32_t ymax, uint32_t stride);
	
	virtual Color GetPixel(uint32_t x, uint32_t y) override;
	virtual void SetPixel(const Color& color, uint32_t x, uint32_t y) override;
	
private:
	Pixel* m_FramebufferData;
	uint32_t m_Stride;
	uint32_t m_Xmin, m_Xmax, m_Ymin, m_Ymax;
};

class EightBitFramebuffer : public Framebuffer
{
public:
	EightBitFramebuffer(uint32_t width, uint32_t height);
	virtual ~EightBitFramebuffer();

	virtual BufferTile* GetBufferTile(uint32_t xmin, uint32_t xmax, uint32_t ymin, uint32_t ymax) override;
	virtual void SaveBufferTile(BufferTile* tile) override;
	virtual bool Ouput(const std::string& file) override;

	virtual bool Parse(const YAML::Node& node) override;
	
private:
	Pixel* m_Data;
};
