//    Copyright 2021 Shaye Garg
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
#include "Core/Material/Material.h"

void Material::NormalMap(const up<Texture>& texture, Interaction& interaction) const
{
    Color c = texture->Evaluate(interaction);
    Vector normal = Vector(c.R(), c.G(), c.B()).Normalize();
    interaction.SNormal = Normal(normal.TransformFrom(
        interaction.Tangent,
        interaction.Bitangent,
        Vector(interaction.GNormal)
    ));
}

bool YAML::convert<up<Material>>::decode(const Node& node, up<Material>& material)
{
	std::string type;
	try { type = node["Type"].as<std::string>(); }
	catch (YAML::Exception& e)
	{
		Error("Material type must be a string (line {})!", e.mark.line + 1);
		return false;
	}
	std::string name;
	try { name = node["Name"].as<std::string>(); }
	catch (YAML::Exception& e)
	{
		Error("Material name must be a string (line {})!", e.mark.line + 1);
		return false;
	}

	try { material = Registry::Get()->GMaterials.at(type)(name); }
	catch (...)
	{
		Error("Material type '{}' does not exist (line {})!", type, node["Type"].Mark().line + 1);
		return false;
	}

	return material->Parse(node);
}
