#include "PCH.h"

#include "Animation.h"

bool operator==(const Keyframe& first, const Keyframe& second)
{
	return first.Time == second.Time;
}

bool operator!=(const Keyframe& first, const Keyframe& second)
{
	return !(first == second);
}

bool operator==(const Keyframe& first, float time)
{
	return first.Time == time;
}

bool operator<(const Keyframe& first, const Keyframe& second)
{
	return first.Time < second.Time;
}

Animation::Animation(const std::vector<Keyframe> keyframes)
	: m_Keyframes(keyframes)
{
	std::sort(m_Keyframes.begin(), m_Keyframes.end());
}

Transform Animation::GetTransform(float time) const
{
	const Keyframe* from = nullptr;
	const Keyframe* to = nullptr;

	bool found = false;

	for (auto it = m_Keyframes.begin(); it != m_Keyframes.end(); it++)
	{
		if (IsNearlyEqual(it->Time, time, 0.01f))
		{
			return Scale(it->Scale) * Rotate(it->Rotation) * Translate(it->Translation);
		}

		if (it->Time > time)
		{
			to = &(*it);
			from = &(*(it - 1));
			found = true;
			break;
		}
	}

	ASSERT(found, "Requested for time outside animation range!");

	float delta = (time - from->Time) / (to->Time - from->Time);

	Vector scale = Lerp(from->Scale, to->Scale, delta);
	Vector translation = Lerp(from->Translation, to->Translation, delta);
	Quaternion rotation = Slerp(from->Rotation, to->Rotation, delta);

	return Scale(scale) * Rotate(rotation) * Translate(translation);
}

float Animation::GetAnimationTime() const
{
	return (m_Keyframes.end() - 1)->Time - m_Keyframes.begin()->Time;
}
