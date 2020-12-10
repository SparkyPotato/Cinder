#pragma once

#include "Core/Scene/Light.h"

class PointLight : public Light
{
public:
	PointLight(const Transform& transform);

	virtual Color EvaluateSample(const RayIntersection& intersection, const std::pair<float, float>& sample, Vector& incoming, float& pdf, Occlusion& tester) const override;

	virtual bool Parse(const YAML::Node& node) override;

	virtual void Preprocess(const Scene& scene) override;

private:
	Color m_Color;
	Point m_Position;
};
