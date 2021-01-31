//    Copyright 2021 Shaye Garg
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
