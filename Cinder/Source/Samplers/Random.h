#pragma once

#include "Core/Components/Sampler.h"

class RandomSampler : public Sampler
{
public:
	RandomSampler();

	virtual float GetSample(float value, float delta) override;

private:
	std::mt19937 m_Random;
};
