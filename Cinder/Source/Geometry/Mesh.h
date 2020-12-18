#pragma once

#include "Core/Scene/Geometry.h"

class Mesh;

struct Vertex
{
	Point Position;
	Normal VNormal;
	Vector Tangent, Bitangent;
	float U, V;
};

class Triangle : public Geometry
{
public:
	Triangle(uint32_t i0, uint32_t i1, uint32_t i2, Mesh* mesh);

	virtual bool Parse(const YAML::Node& node) override { return true; }

	virtual const Bound& GetBound() const override { return m_Bound; }

	virtual bool Intersect(const Ray& ray, Interaction& interaction) const override;
	virtual bool TestIntersect(const Ray& ray) const override;

	virtual float GetArea() const override { return m_Area; }

	virtual const std::vector<Geometry*>& GetSubGeometry() override { return m_Sub; }

	virtual Point Sample(Sampler* sampler, float& pdf) const override;

private:
	std::vector<Geometry*> m_Sub;
	Bound m_Bound;
	float m_Area;
	Vertex& m_V0;
	Vertex& m_V1;
	Vertex& m_V2;
};

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
	
	virtual Point Sample(Sampler* sampler, float& pdf) const override;

private:
	friend class Triangle;

	std::vector<Geometry*> m_Sub;
	std::vector<Vertex> m_Vertices;
	std::vector<Triangle> m_Triangles;
	std::vector<uint32_t> m_SampleList;
	Bound m_Bound;
	float m_Area;
};
