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

#include "Core/Components/Sampler.h"

/// A random sampler.
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
