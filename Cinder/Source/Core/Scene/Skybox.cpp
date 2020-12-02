#include "PCH.h"
#include "Skybox.h"

#include "Material.h"

Color Skybox::Sample(const Vector& direction) const
{
	Vector normal = direction.GetNormalized();

	float dots[6];
	dots[0] = Dot(normal, Vector(0.f, 0.f, 1.f));
	dots[1] = Dot(normal, Vector(0.f, 0.f, -1.f));
	dots[2] = Dot(normal, Vector(-1.f, 0.f, 0.f));
	dots[3] = Dot(normal, Vector(1.f, 0.f, 0.f));
	dots[4] = Dot(normal, Vector(0.f, 1.f, 0.f));
	dots[5] = Dot(normal, Vector(0.f, -1.f, 0.f));

	float dotMax = dots[0];
	int index = 0;
	for (int i = 1; i < 6; i++)
	{
		if (dots[i] > dotMax) { dotMax = dots[i]; index = i; }
	}

	const Texture* sampleFrom;
	Vector faceDirection;
	Vector uVector, vVector;
	switch (index)
	{
	case 0: 
		sampleFrom = &Front;
		faceDirection = Vector(0.f, 0.f, 1.f);
		uVector = Vector(1.f, 0.f, 0.f);
		vVector = Vector(0.f, -1.f, 0.f);
		break;
	case 1: 
		sampleFrom = &Back;
		faceDirection = Vector(0.f, 0.f, -1.f);
		uVector = Vector(-1.f, 0.f, 0.f);
		vVector = Vector(0.f, -1.f, 0.f);
		break;
	case 2: 
		sampleFrom = &Left;
		faceDirection = Vector(-1.f, 0.f, 0.f);
		uVector = Vector(0.f, 0.f, 1.f);
		vVector = Vector(0.f, -1.f, 0.f);
		break;
	case 3: 
		sampleFrom = &Right;
		faceDirection = Vector(1.f, 0.f, 0.f);
		uVector = Vector(0.f, 0.f, -1.f);
		vVector = Vector(0.f, -1.f, 0.f);
		break;
	case 4: 
		sampleFrom = &Top;
		faceDirection = Vector(0.f, 1.f, 0.f);
		uVector = Vector(1.f, 0.f, 0.f);
		vVector = Vector(0.f, 0.f, 1.f);
		break;
	case 5: 
		sampleFrom = &Bottom;
		faceDirection = Vector(0.f, -1.f, 0.f);
		uVector = Vector(1.f, 0.f, 0.f);
		vVector = Vector(0.f, 0.f, 1.f);
		break;
	}

	normal *= (1 / dotMax);
	Vector uv = (Point() + normal) - (Point() + faceDirection);
	float u = Dot(uv, uVector);
	float v = Dot(uv, vVector);
	u = u * 0.5f + 0.5f;
	v = v * 0.5f + 0.5f;

	return Sampler->Sample(*sampleFrom, u, v);
}

Skybox DefaultSkybox()
{
	Skybox box;

	box.Front.Width = 2;
	box.Front.Height = 2;
	box.Front.Data = new Color[4];

	box.Back.Width = 2;
	box.Back.Height = 2;
	box.Back.Data = new Color[4];

	box.Left.Width = 2;
	box.Left.Height = 2;
	box.Left.Data = new Color[4];

	box.Right.Width = 2;
	box.Right.Height = 2;
	box.Right.Data = new Color[4];

	box.Top.Width = 2;
	box.Top.Height = 2;
	box.Top.Data = new Color[4];

	box.Bottom.Width = 2;
	box.Bottom.Height = 2;
	box.Bottom.Data = new Color[4];

	for (int i = 0; i < 4; i++)
	{
		box.Front.Data[i] = Color();
		box.Back.Data[i] = Color();
		box.Left.Data[i] = Color();
		box.Right.Data[i] = Color();
		box.Top.Data[i] = Color();
		box.Bottom.Data[i] = Color();
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

	if (!node["Top"])
	{
		Error("Skybox needs to have a Top (line {})!", node.Mark().line + 1);
		return false;
	}
	LoadTexture(skybox.Top, node["Top"]);

	if (!node["Bottom"])
	{
		Error("Skybox needs to have a Bottom (line {})!", node.Mark().line + 1);
		return false;
	}
	LoadTexture(skybox.Bottom, node["Bottom"]);

	if (!node["Left"])
	{
		Error("Skybox needs to have a Left (line {})!", node.Mark().line + 1);
		return false;
	}
	LoadTexture(skybox.Left, node["Left"]);

	if (!node["Right"])
	{
		Error("Skybox needs to have a Right (line {})!", node.Mark().line + 1);
		return false;
	}
	LoadTexture(skybox.Right, node["Right"]);

	if (!node["Front"])
	{
		Error("Skybox needs to have a Front (line {})!", node.Mark().line + 1);
		return false;
	}
	LoadTexture(skybox.Front, node["Front"]);

	if (!node["Back"])
	{
		Error("Skybox needs to have a Back (line {})!", node.Mark().line + 1);
		return false;
	}
	LoadTexture(skybox.Back, node["Back"]);

	return true;
}
