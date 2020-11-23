#pragma once

#include "Globals.h"

struct RenderSettings
{
	std::string CameraName;
};

namespace YAML
{
template<>
struct convert<RenderSettings>
{
	static bool decode(const Node& node, RenderSettings& settings);
};
}
