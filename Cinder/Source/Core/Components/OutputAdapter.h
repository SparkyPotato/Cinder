#pragma once

#include "Framebuffer.h"

class OutputAdapter
{
public:
	OutputAdapter(const std::string& filename)
		: m_OutputFile(filename)
	{}
	virtual ~OutputAdapter() {}

	virtual bool ParseSettings(const YAML::Node& node) { return true; }

	virtual void WriteOutput(const Framebuffer& framebuffer) = 0;

protected:
	std::filesystem::path m_OutputFile;
};

#define REGISTER_OUTPUT_ADAPTER(name, className) \
OutputAdapter* Spawn##className(const std::string& file) { return new className(file); } \
struct Register##className \
{ \
	Register##className() \
	{ \
		ComponentManager::Get()->RegisterOutputAdapter(#name, &Spawn##className); \
	} \
}; \
static Register##className StaticRegister##className;
