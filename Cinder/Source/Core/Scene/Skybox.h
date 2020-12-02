#pragma once

#include "Core/Components/TextureSampler.h"
#include "Core/Math/Vector.h"

class Skybox
{
public:
	Skybox() = default;

	TextureSampler* Sampler = nullptr;
	Texture Top, Bottom, Right, Left, Front, Back;

	Color Sample(const Vector& direction) const;
};

Skybox DefaultSkybox();

template<>
struct YAML::convert<Skybox>
{
	static bool decode(const Node& node, Skybox& skybox);
};
