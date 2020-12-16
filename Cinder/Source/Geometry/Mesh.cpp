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

	for (uint32_t i = 0; i < mesh->mNumVertices; i++)
	{
		m_Vertices.push_back(
			{ Point(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z),
			Normal(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z),
			Vector(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z),
			Vector(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z),
			mesh->mTextureCoords[i]->x, mesh->mTextureCoords[i]->y }
		);
	}
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

}
