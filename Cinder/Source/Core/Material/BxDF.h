#pragma once

#include "Core/Math/Color.h"
#include "Core/Math/Vector.h"

class BxDF
{
public:
	enum Type
	{
		Reflection = 1 << 0,
		Transmission = 1 << 1,
		Diffuse = 1 << 2,
		Glossy = 1 << 3,
		Specular = 1 << 4,
		All = Reflection | Transmission |
		Diffuse | Glossy | Specular
	};

	BxDF(Type type)
		: m_Type(type)
	{}
	virtual ~BxDF() = default;

	bool IsType(Type type)
	{
		return (m_Type & type) == m_Type;
	}

	virtual Color Evaluate(const Vector& outgoing, const Vector& incoming) const = 0;
	virtual Color EvaluateSample(const Vector& outgoing, Vector& incoming, const std::pair<float, float>& sample, float* pdf) const { return Color(); }

	virtual Color Reflectance(const Vector& outgoing, uint32_t sampleCount, const std::pair<float, float>* samples) const { return Color(); }

protected:
	Type m_Type;
};
