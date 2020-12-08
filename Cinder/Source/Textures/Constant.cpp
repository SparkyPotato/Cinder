#include "PCH.h"
#include "Textures/Constant.h"

Color ConstantTexture::Evaluate(const RayIntersection& intersection)
{
	return m_Color;
}

bool ConstantTexture::Parse(const YAML::Node& node)
{
	if (!node["Color"])
	{
		Error("Constant texture must have a color (line {})!", node.Mark().line + 1);
		return false;
	}
	
	try { m_Color = node["Color"].as<Color>(); }
	catch (YAML::Exception& e)
	{
		return false;
	}
	
	return true;
}
