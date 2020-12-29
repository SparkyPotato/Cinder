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

#include "Core/Scene/Emission.h"
#include "Core/Math/Color.h"

class SimpleEmission : public Emission
{
public:
	SimpleEmission(uint32_t samples);

	virtual Color Evaluate(const Interaction& interaction) const override;
	virtual Color Sample(const Interaction& interaction, Sampler* sampler, Vector& incoming, float& pdf, Occlusion& tester) const override;

	virtual bool Parse(const YAML::Node& node) override;

private:
	Color m_Color;
};
