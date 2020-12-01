#pragma once

#include "Core/Components/TextureSampler.h"

class BilinearSampler : public TextureSampler
{
public:
	Color Sample(const Texture& texture, float u, float v) override;
};
