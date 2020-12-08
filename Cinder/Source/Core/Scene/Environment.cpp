#include "PCH.h"
#include "Core/Scene/Environment.h"

bool YAML::convert<Environment>::decode(const Node& node, Environment& env)
{
	return true;
}
