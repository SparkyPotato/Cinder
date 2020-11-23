#include "RenderSettings.h"

namespace YAML
{
bool convert<RenderSettings>::decode(const Node& node, RenderSettings& settings)
{
	if (node["Camera"]) { settings.CameraName = node["Camera"].as<std::string>(); }

	return true;
}
}
