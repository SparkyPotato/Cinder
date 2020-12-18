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
