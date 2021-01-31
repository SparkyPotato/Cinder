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

/// A single triangle.
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

	virtual Interaction Sample(Sampler* sampler, float& pdf) const override;

private:
	std::vector<Geometry*> m_Sub;
	Bound m_Bound;
	float m_Area;
	Vertex& m_V0;
	Vertex& m_V1;
	Vertex& m_V2;
};

/// A triangle mesh.
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
	
	virtual Interaction Sample(Sampler* sampler, float& pdf) const override;

private:
	friend class Triangle;

	std::vector<Geometry*> m_Sub;
	std::vector<Vertex> m_Vertices;
	std::vector<Triangle> m_Triangles;
	std::vector<uint32_t> m_SampleList;
	Bound m_Bound;
	float m_Area;
};
