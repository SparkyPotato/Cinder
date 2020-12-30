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

#pragma once

#include "Core/Scene/AccelerationStructure.h"

/// No acceleration structure.
/// Tests for intersects by iterating over every object in the scene,
/// Time complexity of O(n).
class None : public AccelerationStructure
{
public:
	virtual bool Parse(const YAML::Node& node) override;

	virtual bool Intersect(const Ray& ray, Interaction& interaction) const override;
	virtual bool TestIntersect(const Ray& ray) const override;

	virtual void Build(const Scene& scene) override;

private:
	const std::vector<Object>* m_Objects;
};
