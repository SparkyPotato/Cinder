#pragma once

#include "Core/Material/Texture.h"

class Cubemap
{
public:
	Cubemap() = default;
	Cubemap(up<Texture> faces[6]);
	
	Color operator()(const Vector& direction) const;
	
private:
	friend struct YAML::convert<Cubemap>;
	
	up<Texture> m_Faces[6];
};

struct FaceData
{
	Normal FaceNormal;
	Vector UVector;
	Vector VVector;
};

const static FaceData CubemapFaceData[6] =
{
	{ Normal(0.f, 0.f, -1.f), Vector(1.f, 0.f, 0.f), Vector(0.f, -1.f, 0.f) },
	{ Normal(0.f, 0.f, 1.f), Vector(-1.f, 0.f, 0.f), Vector(0.f, -1.f, 0.f) },
	{ Normal(1.f, 0.f, 0.f), Vector(0.f, 0.f, 1.f), Vector(0.f, -1.f, 0.f) },
	{ Normal(-1.f, 0.f, 0.f), Vector(0.f, 0.f, -1.f), Vector(0.f, -1.f, 0.f) },
	{ Normal(0.f, -1.f, 0.f), Vector(1.f, 0.f, 0.f), Vector(0.f, 0.f, 1.f) },
	{ Normal(0.f, 1.f, 0.f), Vector(1.f, 0.f, 0.f), Vector(0.f, 0.f, -1.f) }
};

template<>
struct YAML::convert<Cubemap>
{
	static bool decode(const Node& node, Cubemap& cubemap);
};
