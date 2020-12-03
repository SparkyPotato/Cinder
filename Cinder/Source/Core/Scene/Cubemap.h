#pragma once

#include "Core/Components/TextureSampler.h"
#include "Core/Math/Vector.h"

class Cubemap
{
public:
	Cubemap() = default;

	TextureSampler* Sampler = nullptr;
	Texture Faces[6];

	Color operator()(const Vector& direction) const;
	Color operator()(const Normal& direction) const;
};

Cubemap DefaultSkybox();

template<>
struct YAML::convert<Cubemap>
{
	static bool decode(const Node& node, Cubemap& skybox);
};

struct Face
{
	Vector FaceDirection;
	Vector UVector;
	Vector VVector;
};

const static Face CubemapFaceData[6] =
{
	{ Vector(0.f, 0.f, 1.f), Vector(1.f, 0.f, 0.f), Vector(0.f, -1.f, 0.f) },
	{ Vector(0.f, 0.f, -1.f), Vector(-1.f, 0.f, 0.f), Vector(0.f, -1.f, 0.f) },
	{ Vector(-1.f, 0.f, 0.f), Vector(0.f, 0.f, 1.f), Vector(0.f, -1.f, 0.f) },
	{ Vector(1.f, 0.f, 0.f), Vector(0.f, 0.f, -1.f), Vector(0.f, -1.f, 0.f) },
	{ Vector(0.f, 1.f, 0.f), Vector(1.f, 0.f, 0.f), Vector(0.f, 0.f, 1.f) },
	{ Vector(0.f, -1.f, 0.f), Vector(1.f, 0.f, 0.f), Vector(0.f, 0.f, -1.f) }
};