#include "PCH.h"
#include "Samplers/Random.h"

SAMPLER(Random, RandomSampler)

RandomSampler::RandomSampler(uint32_t samples)
	: Sampler(samples), m_Distribution(0.f, 1.f)
{
	std::random_device rd;
	m_Random = std::mt19937(rd());
}

float RandomSampler::Get1D()
{
	return m_Distribution(m_Random);
}

std::pair<float, float> RandomSampler::Get2D()
{
	return { m_Distribution(m_Random), m_Distribution(m_Random) };
}
