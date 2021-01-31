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
#include "Core/Scene/Object.h"

bool Object::Intersect(const Ray& ray, Interaction& interaction) const
{
	Ray r = ToCamera.GetInverse()(ray);
	
	int hit = 0;
	if (m_Geometry->GetSubGeometry().empty())
	{
		hit = m_Geometry->Intersect(r, interaction);
	}
	else
	{
		for (auto geometry : m_Geometry->GetSubGeometry())
		{
			if (geometry->Intersect(r, interaction)) { hit++; }
		}
	}

	if (hit)
	{
		ray.Extent = r.Extent;
		interaction.HitObject = this;
		interaction.HitPoint = ToCamera(interaction.HitPoint);
		interaction.GNormal = interaction.SNormal = ToCamera(interaction.GNormal).Normalize();
	}

	return hit;
}

bool Object::TestIntersect(const Ray& ray) const
{
	Ray r = ToCamera.GetInverse()(ray);

	if (m_Geometry->GetSubGeometry().empty())
	{
		return m_Geometry->TestIntersect(r);
	}
	else
	{
		for (auto geometry : m_Geometry->GetSubGeometry())
		{
			if (geometry->TestIntersect(r)) { return true; }
		}

		return false;
	}
}

Interaction Object::Sample(Sampler* sampler, float& pdf) const
{
	Interaction i = m_Geometry->Sample(sampler, pdf);
	i.HitObject = this;
	i.HitPoint = ToCamera(i.HitPoint);
	i.GNormal = i.SNormal = ToCamera(i.GNormal).Normalize();

	return i;
}

bool YAML::convert<Object>::decode(const Node& node, Object& object)
{
	if (!node["Transform"])
	{
		Error("Object does not have transform (line {})!", node.Mark().line + 1);
		return false;
	}
	if (!node["Geometry"])
	{
		Error("Object does not have geometry (line {})!", node.Mark().line + 1);
		return false;
	}
	
	// Just store object to world, scene will later set it to object to camera
	object.ToCamera = node["Transform"].as<Transform>();
	
	try { object.m_GeometryName = node["Geometry"].as<std::string>(); }
	catch (YAML::Exception& e)
	{
		Error("Geometry reference must be a string (line {})!", e.mark.line + 1);
		return false;
	}

	if (!node["Material"])
	{
		Error("Object does not have a material (line {})!", node.Mark().line + 1);
		return false;
	}

	try { object.m_MaterialName = node["Material"].as<std::string>(); }
	catch (YAML::Exception& e)
	{
		Error("Material reference must be a string (line {})!", e.mark.line + 1);
		return false;
	}
	
	return true;
}
