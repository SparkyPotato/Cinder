#include "PCH.h"
#include "Textures/Constant.h"

TEXTURE(Constant, ConstantTexture)

Color ConstantTexture::Evaluate(const Interaction& interaction) const
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
	catch (...) { return false; }
	
	return true;
}
