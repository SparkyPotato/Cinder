#pragma once

#include "Core/Scene/Light.h"

class PointLight : public Light
{
public:
	PointLight(uint32_t samples, const Transform& transform);

	virtual Color EvaluateSample(const Interaction& interaction, const std::pair<float, float>& sample, Vector& incoming, float& pdf, Occlusion& tester) const override;
	virtual Color EvaluateAlong(const Ray& ray) const override { return Color(); }
	
	virtual bool Parse(const YAML::Node& node) override;

	virtual void Preprocess(const Scene& scene) override;

private:
	Color m_Color;
	Point m_Position;
};
