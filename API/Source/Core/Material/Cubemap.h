//    Copyright 2021 Shaye Garg
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

#include "Core/Material/Texture.h"

/// A collection of 6 textures, forming a cube.
class Cubemap
{
public:
	Cubemap() = default;
	Cubemap(up<Texture> faces[6]);
	
	/// Sample the cubemap.
	///
	/// \param direction Direction to sample the cubemap in.
	/// 
	/// \return The value of the texture function in the sampled direction.
	Color operator()(const Vector& direction) const;
	
private:
	friend struct YAML::convert<Cubemap>;
	
	up<Texture> m_Faces[6];
};

struct FaceData
{
	Normal FaceNormal;
	Vector UVector;
	Vector VVector;
};

const static FaceData CubemapFaceData[6] =
{
	{ Normal(0.f, 0.f, -1.f), Vector(1.f, 0.f, 0.f), Vector(0.f, -1.f, 0.f) },
	{ Normal(0.f, 0.f, 1.f), Vector(-1.f, 0.f, 0.f), Vector(0.f, -1.f, 0.f) },
	{ Normal(1.f, 0.f, 0.f), Vector(0.f, 0.f, 1.f), Vector(0.f, -1.f, 0.f) },
	{ Normal(-1.f, 0.f, 0.f), Vector(0.f, 0.f, -1.f), Vector(0.f, -1.f, 0.f) },
	{ Normal(0.f, -1.f, 0.f), Vector(1.f, 0.f, 0.f), Vector(0.f, 0.f, 1.f) },
	{ Normal(0.f, 1.f, 0.f), Vector(1.f, 0.f, 0.f), Vector(0.f, 0.f, -1.f) }
};

template<>
struct YAML::convert<Cubemap>
{
	static bool decode(const Node& node, Cubemap& cubemap);
};
