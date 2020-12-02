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

bool LoadTexture(Texture& texture, const YAML::Node& node);

template<>
struct YAML::convert<Material>
{
	static bool decode(const Node& node, Material& material);
};
