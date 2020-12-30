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

#include "Core/Material/Texture.h"

/// An 8-bit texture.
class EightBitTexture : public Texture
{
public:
	EightBitTexture() = default;
	EightBitTexture(Color* data, uint32_t width, uint32_t height);
	virtual ~EightBitTexture();

	virtual Color Evaluate(const Interaction& interaction) const override;

	virtual bool Parse(const YAML::Node& node) override;

private:
	Color GetPixel(uint32_t x, uint32_t y) const;

	struct Pixel
	{
		uint8_t R, G, B;
	};

	Pixel* m_Data;
	uint32_t m_Width, m_Height;
};
