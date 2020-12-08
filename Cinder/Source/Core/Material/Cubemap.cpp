#include "PCH.h"
#include "Core/Material/Cubemap.h"

Color Cubemap::operator()(const Vector& direction) const
{
	Vector normalized = direction.GetNormalized();
	
	float dotMax = 0.f;
	int index;
	for (int i = 0; i < 6; i++)
	{
		float dot = Dot(normalized, -CubemapFaceData[i].FaceNormal);
		if (dot > dotMax) { dotMax = dot; index = i; }
	}
	
	normalized *= (1 / dotMax);
	Vector uv = (Point() + normalized) - (Point() - Vector(CubemapFaceData[index].FaceNormal));
	float u = 0.5f * Dot(uv, CubemapFaceData[index].UVector) + 0.5f;
	float v = 0.5f * Dot(uv, CubemapFaceData[index].VVector) + 0.5f;
	
	
	RayIntersection r;
	r.HitNormal = CubemapFaceData[index].FaceNormal;
	r.HitPoint = Point() + normalized;
	r.HitObject = nullptr;
	r.U = u;
	r.V = v;
	
	return m_Faces[index]->Evaluate(r);
}

bool YAML::convert<Cubemap>::decode(const Node& node, Cubemap& cubemap)
{
	if (!node["Front"])
	{
		Error("Cubemap needs to have a front (line {})!", node.Mark().line + 1);
		return false;
	}
	if (!node["Back"])
	{
		Error("Cubemap needs to have a back (line {})!", node.Mark().line + 1);
		return false;
	}
	if (!node["Left"])
	{
		Error("Cubemap needs to have a left (line {})!", node.Mark().line + 1);
		return false;
	}
	if (!node["Right"])
	{
		Error("Cubemap needs to have a right (line {})!", node.Mark().line + 1);
		return false;
	}
	if (!node["Top"])
	{
		Error("Cubemap needs to have a top (line {})!", node.Mark().line + 1);
		return false;
	}
	if (!node["Bottom"])
	{
		Error("Cubemap needs to have a bottom (line {})!", node.Mark().line + 1);
		return false;
	}
	
	try
	{
		cubemap.m_Faces[0] = node["Front"].as<up<Texture>>();
		cubemap.m_Faces[1] = node["Back"].as<up<Texture>>();
		cubemap.m_Faces[2] = node["Left"].as<up<Texture>>();
		cubemap.m_Faces[3] = node["Right"].as<up<Texture>>();
		cubemap.m_Faces[4] = node["Top"].as<up<Texture>>();
		cubemap.m_Faces[5] = node["Bottom"].as<up<Texture>>();
	}
	catch (...) { return false; }
	
	return true;
}
