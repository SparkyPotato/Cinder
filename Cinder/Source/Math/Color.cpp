#include "Color.h"

Color::Color()
	: R(0.f), G(0.f), B(0.f)
{}

Color::Color(float r, float g, float b)
	: R(r), G(g), B(b)
{}

namespace YAML
{
bool convert<Color>::decode(const Node& node, Color& color)
{
	if (!node["R"])
	{
		Error("Color does not have R! ("
			COLOR, "line ", node.Mark().line, "\x1b[31m).");
	}
	if (!node["G"])
	{
		Error("Color does not have G! ("
			COLOR, "line ", node.Mark().line, "\x1b[31m).");
	}
	if (!node["B"])
	{
		Error("Color does not have B! ("
			COLOR, "line ", node.Mark().line, "\x1b[31m).");
	}

	color = Color(
		node["R"].as<float>(),
		node["G"].as<float>(),
		node["B"].as<float>()
	);

	return true;
}
}
