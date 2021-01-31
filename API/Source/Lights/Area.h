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

#pragma once

#include "Core/Scene/Light.h"
#include "Core/Scene/Object.h"
#include "Core/Material/Texture.h"

/// An area light which is attached to an object.
class AreaLight : public Light
{
public:
	AreaLight(uint32_t samples, const Texture* emission, float intensity, const Object* object);

	virtual Color Sample(const Interaction& interaction, Sampler* sampler, Vector& incoming, float& pdf, Occlusion& tester) const override;
	virtual Color EvaluateAlong(const Ray& ray) const override;

	virtual bool Parse(const YAML::Node& node) override;

private:
	const Texture* m_Color;
	float m_Intensity;
	const Object* m_Object;
};
