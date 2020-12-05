#include "PCH.h"
#include "Samplers/Random.h"

SAMPLER(Random, RandomSampler)

RandomSampler::RandomSampler()
{
	std::random_device rd;
	m_Random = std::mt19937(rd());
}

float RandomSampler::GetSample(float value, float delta)
{
	std::uniform_real_distribution<float> distribution(-delta, delta);

	return value + distribution(m_Random);
}
