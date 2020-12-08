#include "PCH.h"
#include "Core/Scene/Environment.h"

Color Environment::Sample(const Vector& direction) const
{
	return m_Skybox(m_CameraToWorld(direction));
}

bool YAML::convert<Environment>::decode(const Node& node, Environment& env)
{
	try { env.m_Skybox = node.as<Cubemap>(); }
	catch (...) { return false; }

	return true;
}
