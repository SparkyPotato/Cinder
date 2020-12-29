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

class Sampler
{
public:
	Sampler(uint32_t samplesPerPixel)
		: m_SamplesPerPixel(samplesPerPixel)
	{}
	virtual ~Sampler() = default;

	virtual void StartPixel(uint32_t x, uint32_t y) 
	{ m_CurrentX = x; m_CurrentY = y; m_CurrentSample = 0; }

	virtual float Get1D() = 0;
	virtual std::pair<float, float> Get2D() = 0;

	virtual bool NextSample() { m_CurrentSample++; return !(m_CurrentSample > m_SamplesPerPixel); }

protected:
	uint32_t m_SamplesPerPixel;
	uint32_t m_CurrentX = 0, m_CurrentY = 0;
	uint32_t m_CurrentSample = 0;
};

#define SAMPLER(type, className) \
up<Sampler> Spawn##className(uint32_t samples) { return std::make_unique<className>(samples); } \
struct Register##className \
{ \
	Register##className() { Registry::Get()->GSamplers.emplace(#type, &Spawn##className); } \
}; \
static Register##className StaticRegister##className;
