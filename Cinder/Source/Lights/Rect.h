#pragma once

#include "Core/Scene/Light.h"

class RectLight : public Light
{
public:
	RectLight(const Transform& transform);

	virtual Color EvaluateSample(const Interaction& interaction, const std::pair<float, float>& sample, Vector& incoming, float& pdf, Occlusion& tester) const override;
	virtual Color EvaluateAlong(const Ray& ray) const override;
	
	virtual bool Parse(const YAML::Node& node) override;

	virtual void Preprocess(const Scene& scene) override;

private:
    Color m_Color;
    Vector m_Size;
    float m_Area;
};
