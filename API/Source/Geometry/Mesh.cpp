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
#include "Geometry/Mesh.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

GEOMETRY(Mesh, Mesh)

Mesh::Mesh(const std::string& name)
	: Geometry(name)
{}

bool Mesh::Parse(const YAML::Node& node)
{
	if (!node["Source"])
	{
		Error("Mesh does not have a source (line {})!", node.Mark().line + 1);
		return false;
	}
	std::string source;
	try { source = node["Source"].as<std::string>(); }
	catch (YAML::Exception& e)
	{
		Error("Mesh source must be a string (line {})!", e.mark.line + 1);
		return false;
	}

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(
		source,
		aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_GenBoundingBoxes
	);

	if (!scene)
	{
		Error("Mesh load error: {} (line {})", importer.GetErrorString(), node.Mark().line + 1);
		return false;
	}

	const aiMesh* mesh = scene->mMeshes[0];
	if (!mesh->HasNormals()) 
	{ Error("Mesh does not have normals (line {})!", node.Mark().line + 1); return false; }
	if (!mesh->HasTangentsAndBitangents()) 
	{ Error("Mesh does not have tangents or bitangents (line {})!", node.Mark().line + 1); return false; }
	if (!mesh->HasTextureCoords(0)) 
	{ Error("Mesh does not have texture coordinates (line {})!", node.Mark().line + 1); return false; }

	m_Bound = Bound(
		{ mesh->mAABB.mMin.x, mesh->mAABB.mMin.y, mesh->mAABB.mMin.z },
		{ mesh->mAABB.mMax.x, mesh->mAABB.mMax.y, mesh->mAABB.mMax.z }
	);

	m_Vertices.reserve(mesh->mNumVertices);
	for (uint32_t i = 0; i < mesh->mNumVertices; i++)
	{
		auto& v = m_Vertices.emplace_back();
		v.Position = Point(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		v.VNormal = Normal(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		v.Tangent = Vector(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
		v.Bitangent = Vector(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
		v.U = mesh->mTextureCoords[0][i].x;
		v.V = mesh->mTextureCoords[0][i].y;
	}

	m_Area = 0.f;

	float minArea = FloatMax;
	std::vector<std::pair<uint32_t, float>> areas;

	m_Triangles.reserve(mesh->mNumFaces);
	for (uint32_t i = 0; i < mesh->mNumFaces; i++)
	{
		if (mesh->mFaces[i].mNumIndices == 3)
		{
			auto& t = m_Triangles.emplace_back(
				mesh->mFaces[i].mIndices[0],
				mesh->mFaces[i].mIndices[1],
				mesh->mFaces[i].mIndices[2],
				this
			);

			if (t.GetArea() < minArea) { minArea = t.GetArea(); }

			areas.push_back({ uint32_t(m_Triangles.size()) - 1, t.GetArea() });
			m_Area += t.GetArea();
			m_Sub.push_back(&t);
		}
	}

	for (auto& area : areas)
	{
		uint32_t relative = uint32_t(area.second /= minArea);

		for (; relative > 0.f; relative--) { m_SampleList.push_back(area.first); }
	}

	return true;
}

bool Mesh::Intersect(const Ray& ray, Interaction& interaction) const
{
	ASSERT(false, "Mesh should not be directly intersected!");
	return false;
}

bool Mesh::TestIntersect(const Ray& ray) const
{
	ASSERT(false, "Mesh should not be directly intersected!");
	return false;
}

float Mesh::GetArea() const
{
	return m_Area;
}

Interaction Mesh::Sample(Sampler* sampler, float& pdf) const
{
	uint64_t tri = std::min(uint64_t(sampler->Get1D() * m_SampleList.size()), m_SampleList.size() - 1);

	pdf = 1.f / m_Area;
	float d;
	return m_Triangles[m_SampleList[tri]].Sample(sampler, d);
}

Triangle::Triangle(uint32_t i0, uint32_t i1, uint32_t i2, Mesh* mesh)
	: Geometry(""),
	m_V0(mesh->m_Vertices[i0]), m_V1(mesh->m_Vertices[i1]), m_V2(mesh->m_Vertices[i2])
{
	m_Area = 0.5f * Cross(m_V1.Position - m_V0.Position, m_V2.Position - m_V0.Position).GetLength();

	m_Bound = Bound(m_V0.Position, m_V1.Position);
	m_Bound = Union(m_Bound, m_V2.Position);
}

template<typename T>
T BarycentricInterp(float u, float v, float w, const T& v0, const T& v1, const T& v2)
{
	return u * v0 + v * v1 + w * v2;
}

bool Triangle::Intersect(const Ray& ray, Interaction& interaction) const
{
	Vector p0t = m_V0.Position - ray.Origin;
	Vector p1t = m_V1.Position - ray.Origin;
	Vector p2t = m_V2.Position - ray.Origin;

	int kz = ray.Direction.GetAbs().GetMaxDimension();
	int kx = kz + 1; if (kx == 3) { kx = 0; }
	int ky = kx + 1; if (ky == 3) { ky = 0; }
	Vector d = Shuffle(ray.Direction, kx, ky, kz);
	p0t = Shuffle(p0t, kx, ky, kz);
	p1t = Shuffle(p1t, kx, ky, kz);
	p2t = Shuffle(p2t, kx, ky, kz);

	float Sx = -d.X() / d.Z();
	float Sy = -d.Y() / d.Z();
	float Sz = 1.f / d.Z();
	p0t.X() += Sx * p0t.Z();
	p0t.Y() += Sy * p0t.Z();
	p1t.X() += Sx * p1t.Z();
	p1t.Y() += Sy * p1t.Z();
	p2t.X() += Sx * p2t.Z();
	p2t.Y() += Sy * p2t.Z();

	double p2txp1ty = (double)p2t.X() * (double)p1t.Y();
	double p2typ1tx = (double)p2t.Y() * (double)p1t.X();
	float e0 = (float)(p2typ1tx - p2txp1ty);
	double p0txp2ty = (double)p0t.X() * (double)p2t.Y();
	double p0typ2tx = (double)p0t.Y() * (double)p2t.X();
	float e1 = (float)(p0typ2tx - p0txp2ty);
	double p1txp0ty = (double)p1t.X() * (double)p0t.Y();
	double p1typ0tx = (double)p1t.Y() * (double)p0t.X();
	float e2 = (float)(p1typ0tx - p1txp0ty);

	if ((e0 < 0.f || e1 < 0.f || e2 < 0.f) && (e0 > 0.f || e1 > 0.f || e2 > 0.f)) { return false; }
	float det = e0 + e1 + e2;
	if (det == 0.f) { return false; }

	p0t.Z() *= Sz;
	p1t.Z() *= Sz;
	p2t.Z() *= Sz;
	float tScaled = e0 * p0t.Z() + e1 * p1t.Z() + e2 * p2t.Z();
	if (det < 0 && (tScaled >= 0 || tScaled < ray.Extent * det)) { return false; }
	else if (det > 0 && (tScaled <= 0 || tScaled > ray.Extent * det)) { return false; }

	float invDet = 1 / det;
	float u = e0 * invDet;
	float v = e1 * invDet;
	float w = e2 * invDet;
	float t = tScaled * invDet;
	if (t > ray.Extent) { return false; }
	ray.Extent = t;

	interaction.HitPoint = ray.Origin + ray.Direction * t;
	interaction.GNormal = Normal(BarycentricInterp(u, v, w, Vector(m_V0.VNormal), Vector(m_V1.VNormal), Vector(m_V2.VNormal)));
	interaction.Tangent = BarycentricInterp(u, v, w, m_V0.Tangent, m_V1.Tangent, m_V2.Tangent);
	interaction.Bitangent = BarycentricInterp(u, v, w, m_V0.Bitangent, m_V1.Bitangent, m_V2.Bitangent);
	interaction.U = BarycentricInterp(u, v, w, m_V0.U, m_V1.U, m_V2.U);
	interaction.V = BarycentricInterp(u, v, w, m_V0.V, m_V1.V, m_V2.V);

	return true;
}

bool Triangle::TestIntersect(const Ray& ray) const
{
	Vector p0t = m_V0.Position - ray.Origin;
	Vector p1t = m_V1.Position - ray.Origin;
	Vector p2t = m_V2.Position - ray.Origin;

	int kz = ray.Direction.GetAbs().GetMaxDimension();
	int kx = kz + 1; if (kx == 3) { kx = 0; }
	int ky = kx + 1; if (ky == 3) { ky = 0; }
	Vector d = Shuffle(ray.Direction, kx, ky, kz);
	p0t = Shuffle(p0t, kx, ky, kz);
	p1t = Shuffle(p1t, kx, ky, kz);
	p2t = Shuffle(p2t, kx, ky, kz);

	float Sx = -d.X() / d.Z();
	float Sy = -d.Y() / d.Z();
	float Sz = 1.f / d.Z();
	p0t.X() += Sx * p0t.Z();
	p0t.Y() += Sy * p0t.Z();
	p1t.X() += Sx * p1t.Z();
	p1t.Y() += Sy * p1t.Z();
	p2t.X() += Sx * p2t.Z();
	p2t.Y() += Sy * p2t.Z();

	double p2txp1ty = (double)p2t.X() * (double)p1t.Y();
	double p2typ1tx = (double)p2t.Y() * (double)p1t.X();
	float e0 = (float)(p2typ1tx - p2txp1ty);
	double p0txp2ty = (double)p0t.X() * (double)p2t.Y();
	double p0typ2tx = (double)p0t.Y() * (double)p2t.X();
	float e1 = (float)(p0typ2tx - p0txp2ty);
	double p1txp0ty = (double)p1t.X() * (double)p0t.Y();
	double p1typ0tx = (double)p1t.Y() * (double)p0t.X();
	float e2 = (float)(p1typ0tx - p1txp0ty);

	if ((e0 < 0.f || e1 < 0.f || e2 < 0.f) && (e0 > 0.f || e1 > 0.f || e2 > 0.f)) { return false; }
	float det = e0 + e1 + e2;
	if (det == 0.f) { return false; }

	p0t.Z() *= Sz;
	p1t.Z() *= Sz;
	p2t.Z() *= Sz;
	float tScaled = e0 * p0t.Z() + e1 * p1t.Z() + e2 * p2t.Z();
	if (det < 0 && (tScaled >= 0 || tScaled < ray.Extent * det)) { return false; }
	else if (det > 0 && (tScaled <= 0 || tScaled > ray.Extent * det)) { return false; }

	float invDet = 1 / det;
	float u = e0 * invDet;
	float v = e1 * invDet;
	float w = e2 * invDet;
	float t = tScaled * invDet;
	if (t > ray.Extent) { return false; }

	return true;
}

Interaction Triangle::Sample(Sampler* sampler, float& pdf) const
{
	pdf = 1.f / m_Area;
	auto sample = sampler->Get2D();

	float su0 = std::sqrt(sample.first);
	float u = 1.f - su0;
	float v = sample.second * su0;
	float w = 1.f - u - v;

	Vector p0, p1, p2;
	p0 = m_V0.Position - Point();
	p1 = m_V1.Position - Point();
	p2 = m_V2.Position - Point();

	Interaction i;
	i.HitPoint = Point() + BarycentricInterp(u, v, w, p0, p1, p2);
	i.GNormal = Normal(BarycentricInterp(u, v, w, Vector(m_V0.VNormal), Vector(m_V1.VNormal), Vector(m_V2.VNormal)));
	i.Tangent = BarycentricInterp(u, v, w, m_V0.Tangent, m_V1.Tangent, m_V2.Tangent);
	i.Bitangent = BarycentricInterp(u, v, w, m_V0.Bitangent, m_V1.Bitangent, m_V2.Bitangent);
	i.U = BarycentricInterp(u, v, w, m_V0.U, m_V1.U, m_V2.U);
	i.V = BarycentricInterp(u, v, w, m_V0.V, m_V1.V, m_V2.V);

	return i;
}
