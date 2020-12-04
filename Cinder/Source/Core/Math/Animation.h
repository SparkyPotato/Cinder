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
