#pragma once

#include "Core/Material/Cubemap.h"
#include "Core//Math/Transform.h"

class Environment
{
public:
	Environment() = default;
	
	Color Sample(const Vector& direction) const;

private:
	friend struct YAML::convert<Environment>;
	friend class Scene;

	Transform m_CameraToWorld;
	Cubemap m_Skybox;
};

template<>
struct YAML::convert<Environment>
{
	static bool decode(const Node& node, Environment& environment);
};
