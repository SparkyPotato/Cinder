//    Copyright 2021 SparkyPotato
//
//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
//    
//    http://www.apache.org/licenses/LICENSE-2.0
//    
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.

#include "PCH.h"
#include "AccelerationStructures/None.h"

ACCELERATION(None, None)

bool None::Parse(const YAML::Node& node)
{
	return true;
}

bool None::Intersect(const Ray& ray, Interaction& interaction) const
{
	int ret = 0;

	for (const auto& object : *m_Objects)
	{
		if (object.Intersect(ray, interaction)) { ret++; }
	}

	return ret > 0;
}

bool None::TestIntersect(const Ray& ray) const
{
	for (const auto& object : *m_Objects)
	{
		if (object.TestIntersect(ray)) { return true; }
	}

	return false;
}

void None::Build(const Scene& scene)
{
	m_Objects = &scene.GetObjects();
}
