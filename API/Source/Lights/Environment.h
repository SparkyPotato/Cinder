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

#include "Core/Scene/Light.h"
#include "Core/Material/Cubemap.h"

/// A directional light which emits radiance in a single direction.
class Environment : public Light
{
public:
	Environment(uint32_t samples, const Transform& transform);

	virtual Color Sample(const Interaction& interaction, Sampler* sampler, Vector& incoming, float& pdf, Occlusion& tester) const override;
	virtual Color EvaluateAlong(const Ray& ray) const override;

	virtual bool Parse(const YAML::Node& node) override;

	virtual void Preprocess(const Scene& scene) override;

private:
	struct IrradianceSample
	{
		Color Coefficients[9];

		IrradianceSample& operator+=(const IrradianceSample& other);
		IrradianceSample operator*=(const IrradianceSample& other);
		IrradianceSample& operator*=(const Color& other);
	};

	void ComputeIrradiance();

	IrradianceSample GetAverageIrradiance(const Cubemap& cubemap);
	void GenerateIrradiance(const IrradianceSample& average);

	uint32_t m_IrradianceResolution;
	Transform m_CameraToWorld;
	Cubemap m_Skybox;
	Cubemap m_Irradiance;
};
