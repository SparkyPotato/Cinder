#pragma once

#include "Core/Scene/Emission.h"
#include "Core/Material/Texture.h"

class SimpleEmission : public Emission
{
public:
	SimpleEmission(uint32_t samples);

	virtual Color Evaluate(const Interaction& interaction) const override;
	virtual Color Sample(const Interaction& interaction, Sampler* sampler, Vector& incoming, float& pdf, Occlusion& tester) const override;

	virtual bool Parse(const YAML::Node& node) override;

private:
	up<Texture> m_Color;
	float m_Intensity = 0.f;
};
