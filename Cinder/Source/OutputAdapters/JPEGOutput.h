#pragma once

#include "Core/Components/OutputAdapter.h"

class JPEGOuput : public OutputAdapter
{
public:
	JPEGOuput(const std::string& filename);

	bool ParseSettings(const YAML::Node& node) override;

	void WriteOutput(const Framebuffer& framebuffer) override;

private:
	int m_Quality = 50;
};
