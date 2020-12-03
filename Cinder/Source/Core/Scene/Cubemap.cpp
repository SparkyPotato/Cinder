#include "PCH.h"
#include "Cubemap.h"

#include "Material.h"

Color Cubemap::operator()(const Vector& direction) const
{
	Vector normalized = direction.GetNormalized();

	float dotMax = 0.f;
	int index;
	for (int i = 0; i < 6; i++)
	{
		float dot = Dot(normalized, CubemapFaceData[i].FaceDirection);
		if (dot > dotMax) { dotMax = dot; index = i; }
	}

	normalized *= (1 / dotMax);
	Vector uv = (Point() + normalized) - (Point() + CubemapFaceData[index].FaceDirection);
	float u = 0.5f * Dot(uv, CubemapFaceData[index].UVector) + 0.5f;
	float v = 0.5f * Dot(uv, CubemapFaceData[index].VVector) + 0.5f;

	return Sampler->Sample(Faces[index], u, v);
}

Color Cubemap::operator()(const Normal& direction) const
{
	return (*this)(Vector(direction));
}

Cubemap DefaultSkybox()
{
	Cubemap box;

	for (int i = 0; i < 6; i++)
	{
		box.Faces[i].Width = 2;
		box.Faces[i].Height = 2;
		box.Faces[i].Data = Memory::Get()->AllocateArr<Color>(4);
	}

	for (int i = 0; i < 4; i++)
	{
		box.Faces[0].Data[i] = Color();
		box.Faces[1].Data[i] = Color();
		box.Faces[2].Data[i] = Color();
		box.Faces[3].Data[i] = Color();
		box.Faces[4].Data[i] = Color();
		box.Faces[5].Data[i] = Color();
	}

	return box;
}

bool YAML::convert<Cubemap>::decode(const Node& node, Cubemap& skybox)
{
	std::string sampler = "Bilinear";
	if (!node["Sampler"])
	{
		Warning("No Sampler specified,  using default '{}' (line {})!", sampler, node.Mark().line + 1);
	}

	try { sampler = node["Sampler"].as<std::string>(); }
	catch (YAML::Exception& e)
	{
		Error("Sampler must be a string (line {})!", e.mark.line + 1);
		return false;
	}

	try { skybox.Sampler = ComponentManager::Get()->SpawnTextureSampler(sampler); }
	catch (...)
	{
		Error("Sampler '{}' does not exist (line {})!", sampler, node["Sampler"].Mark().line + 1);
		return false;
	}

	if (!node["Front"])
	{
		Error("Skybox needs to have a Front (line {})!", node.Mark().line + 1);
		return false;
	}
	LoadTexture(skybox.Faces[0], node["Front"]);

	if (!node["Back"])
	{
		Error("Skybox needs to have a Back (line {})!", node.Mark().line + 1);
		return false;
	}
	LoadTexture(skybox.Faces[1], node["Back"]);

	if (!node["Left"])
	{
		Error("Skybox needs to have a Left (line {})!", node.Mark().line + 1);
		return false;
	}
	LoadTexture(skybox.Faces[2], node["Left"]);

	if (!node["Right"])
	{
		Error("Skybox needs to have a Right (line {})!", node.Mark().line + 1);
		return false;
	}
	LoadTexture(skybox.Faces[3], node["Right"]);

	if (!node["Top"])
	{
		Error("Skybox needs to have a Top (line {})!", node.Mark().line + 1);
		return false;
	}
	LoadTexture(skybox.Faces[4], node["Top"]);

	if (!node["Bottom"])
	{
		Error("Skybox needs to have a Bottom (line {})!", node.Mark().line + 1);
		return false;
	}
	LoadTexture(skybox.Faces[5], node["Bottom"]);

	return true;
}
