#pragma once

#include "Core/Components/OutputAdapter.h"

class PNGOutput : public OutputAdapter
{
public:
	PNGOutput(const std::string& filename);

	void WriteOutput(const Framebuffer& framebuffer) override;
};
