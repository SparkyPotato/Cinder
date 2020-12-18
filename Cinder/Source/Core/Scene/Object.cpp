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
			if (m_Geometry->TestIntersect(r)) { return true; }
		}

		return false;
	}
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

	if (node["Emission"])
	{
		const auto& n = node["Emission"];
		if (!n["Type"])
		{
			Error("Emission must have a type (line {})!", n.Mark().line + 1);
			return false;
		}
		std::string type;
		try { type = n["Type"].as<std::string>(); }
		catch (YAML::Exception& e)
		{
			Error("Emission type must be a string (line {})!", e.mark.line + 1);
			return false;
		}

		if (!n["Samples"])
		{
			Error("Emission must have a sample count (line {})!", n.Mark().line + 1);
			return false;
		}
		uint32_t samples;
		try { samples = n["Samples"].as<uint32_t>(); }
		catch (YAML::Exception& e)
		{
			Error("Emission sample count must be an unsigned integer (line {})!", e.mark.line + 1);
			return false;
		}

		try { object.m_Emission = Registry::Get()->GEmission.at(type)(samples); }
		catch (...)
		{
			Error("Emission type '{}' does not exist (line {})!", type, n.Mark().line + 1);
			return false;
		}

		return object.m_Emission->Parse(n);
	}
	else
	{
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
	}
	
	return true;
}
