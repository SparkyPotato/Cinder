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

#include "Core/Math/Quaternion.h"
#include "Core/Math/Transform.h"

struct Keyframe
{
public:
	float Time = 0.f;
	Vector Translation;
	Vector Scale;
	Quaternion Rotation;
};

bool operator==(const Keyframe& first, const Keyframe& second);
bool operator!=(const Keyframe& first, const Keyframe& second);
bool operator<(const Keyframe& first, const Keyframe& second);

class Animation
{
public:
	Animation() = default;
	Animation(const std::vector<Keyframe> keyframes);

	Transform GetTransform(float time) const;
	float GetAnimationTime() const;

private:
	std::vector<Keyframe> m_Keyframes;
};
