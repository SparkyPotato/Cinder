#pragma once

#include "Core/Components/Registry.h"

class Sampler
{
public:
	Sampler(uint32_t samplesPerPixel)
		: m_SamplesPerPixel(samplesPerPixel)
	{}
	virtual ~Sampler() {}

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
