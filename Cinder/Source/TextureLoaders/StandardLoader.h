#pragma once
#include "Core/Components/TextureLoader.h"

class StandardLoader : public TextureLoader
{
public:
	Texture LoadTexture(const std::string& filePath) override;
};
