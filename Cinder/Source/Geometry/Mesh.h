#pragma once

#include "Core/Scene/Geometry.h"

class Mesh : public Geometry
{
public:
	Mesh(const std::string& name);

	virtual bool Parse(const YAML::Node& node) override;

	virtual const Bound& GetBound() const override { return m_Bound; }

	virtual bool Intersect(const Ray& ray, Interaction& interaction) const override;
	virtual bool TestIntersect(const Ray& ray) const override;

	virtual float GetArea() const override;

	virtual const std::vector<Geometry*>& GetSubGeometry() override { return m_Sub; }

	struct Vertex
	{
		Point Position;
		Normal VNormal;
		Vector Tangent, Bitangent;
		float U, V;
	};
	
private:
	std::vector<Geometry*> m_Sub;
	std::vector<Vertex> m_Vertices;
	Bound m_Bound;
};
