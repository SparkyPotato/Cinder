#pragma once

#include "Core/Components/Sampler.h"

class RandomSampler : public Sampler
{
public:
	RandomSampler(uint32_t samples);

	virtual float Get1D() override;
	virtual std::pair<float, float> Get2D() override;

private:
	std::mt19937 m_Random;
	std::uniform_real_distribution<float> m_Distribution;
};
