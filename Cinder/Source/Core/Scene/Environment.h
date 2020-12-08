#pragma once

class Environment
{
public:
	Environment() = default;
	
private:
	friend struct YAML::convert<Environment>;
};

template<>
struct YAML::convert<Environment>
{
	static bool decode(const Node& node, Environment& environment);
};
