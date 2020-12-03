#pragma once

#include "Core/Components/TextureSampler.h"

class Material
{
public:
	Material() = default;
	
	Color SampleAlbedo(float u, float v);

	std::string Name;
	TextureSampler* Sampler;

	Texture Albedo;
};

template<>
struct YAML::convert<Material>
{
	static bool decode(const Node& node, Material& material);
};
