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

#include "Core/Scene/Geometry.h"

class Sphere : public Geometry
{
public:
	Sphere(const std::string& name);

	virtual bool Parse(const YAML::Node& node) override;

	virtual const Bound& GetBound() const override { return m_Bound; }

	virtual bool Intersect(const Ray& ray, Interaction& interaction) const override;
	virtual bool TestIntersect(const Ray& ray) const override;

	virtual float GetArea() const override;

	virtual const std::vector<Geometry*>& GetSubGeometry() override { return m_Sub; }

	virtual Interaction Sample(Sampler* sampler, float& pdf) const override;

private:
	float m_Radius = 0.f;
	float m_Area;
	std::vector<Geometry*> m_Sub;
    Bound m_Bound;
};
