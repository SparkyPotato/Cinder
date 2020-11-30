#pragma once

#include "Sampler.h"

class PreviewRenderer : public Sampler
{
public:
	PreviewRenderer();

	bool ParseSettings(const YAML::Node& node) override;
};
