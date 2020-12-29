//    Copyright 2021 SparkyPotato
//
//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
//    
//    http://www.apache.org/licenses/LICENSE-2.0
//    
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.

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
