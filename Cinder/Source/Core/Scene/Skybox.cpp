#include "PCH.h"
#include "Skybox.h"

#include "Material.h"

struct Face
{
	Vector FaceDirection;
	Vector UVector;
	Vector VVector;
};

static Face FaceData[6] =
{
	{ Vector(0.f, 0.f, 1.f), Vector(1.f, 0.f, 0.f), Vector(0.f, -1.f, 0.f) },
	{ Vector(0.f, 0.f, -1.f), Vector(-1.f, 0.f, 0.f), Vector(0.f, -1.f, 0.f) },
	{ Vector(-1.f, 0.f, 0.f), Vector(0.f, 0.f, 1.f), Vector(0.f, -1.f, 0.f) },
	{ Vector(1.f, 0.f, 0.f), Vector(0.f, 0.f, -1.f), Vector(0.f, -1.f, 0.f) },
	{ Vector(0.f, 1.f, 0.f), Vector(1.f, 0.f, 0.f), Vector(0.f, 0.f, 1.f) },
	{ Vector(0.f, -1.f, 0.f), Vector(1.f, 0.f, 0.f), Vector(0.f, 0.f, -1.f) }
};

Color Skybox::Sample(const Vector& direction) const
{
	Vector normal = direction.GetNormalized();

	float dots[6];
	dots[0] = Dot(normal, FaceData[0].FaceDirection);
	dots[1] = Dot(normal, FaceData[1].FaceDirection);
	dots[2] = Dot(normal, FaceData[2].FaceDirection);
	dots[3] = Dot(normal, FaceData[3].FaceDirection);
	dots[4] = Dot(normal, FaceData[4].FaceDirection);
	dots[5] = Dot(normal, FaceData[5].FaceDirection);

	float dotMax = dots[0];
	int index = 0;
	for (int i = 1; i < 6; i++)
	{
		if (dots[i] > dotMax) { dotMax = dots[i]; index = i; }
	}

	normal *= (1 / dotMax);
	Vector uv = (Point() + normal) - (Point() + FaceData[index].FaceDirection);
	float u = 0.5f * Dot(uv, FaceData[index].UVector) + 0.5f;
	float v = 0.5f * Dot(uv, FaceData[index].VVector) + 0.5f;

	return Sampler->Sample(Faces[index], u, v);
}

Skybox DefaultSkybox()
{
	Skybox box;

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

bool YAML::convert<Skybox>::decode(const Node& node, Skybox& skybox)
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
