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

#include "PCH.h"
#include "Core/Math/Animation.h"

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
