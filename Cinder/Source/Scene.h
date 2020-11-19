#pragma once

#include "json/json.hpp"

struct Scene
{

};

void from_json(const nlohmann::json& j, Scene& scene);
