#pragma once

#include "Core/Math/Color.h"
#include "Core/Math/Vector.h"
#include "Core/Math/Sampling.h"

inline float Cos(const Vector& w) { return w.Z(); }
inline float Cos2(const Vector& w) { return w.Z() * w.Z(); }
inline float AbsCos(const Vector& w) { return std::abs(w.Z()); }

inline float Sin2(const Vector& w) { return std::max(0.f, 1.f - Cos2(w)); }
inline float Sin(const Vector& w) { return std::sqrt(Sin2(w)); }

inline float Tan(const Vector& w) { return Sin(w) / Cos(w); }
inline float Tan2(const Vector& w) { return Sin2(w) / Cos2(w); }

inline float CosPhi(const Vector &w)
{
	float sin = Sin(w);
	return (sin == 0.f) ? 1.f : w.X() / sin;
}
inline float Cos2Phi(const Vector& w) { return CosPhi(w) * CosPhi(w); }

inline float SinPhi(const Vector& w)
{
	float sin = Sin(w);
	return (sin == 0.f) ? 0.f : w.Y() / sin;
}
inline float Sin2Phi(const Vector& w) { return SinPhi(w) * SinPhi(w); }

inline Vector Reflect(const Vector& wo, const Vector& n)
{
	return -wo + 2 * Dot(wo, n) * n;
}

inline bool SameHemisphere(const Vector& w, const Vector& wp) { return w.Z() * wp.Z() > 0; }
inline bool SameHemisphere(const Vector& w, const Normal& wp) { return w.Z() * wp.Z() > 0; }

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
		return m_Type & type;
	}
	
	Type GetType() const { return m_Type; }

	virtual Color Evaluate(const Vector& outgoing, const Vector& incoming) const = 0;
	virtual Color EvaluateSample(const Vector& outgoing, Vector& incoming, const std::pair<float, float>& sample, float& pdf) const
	{
		incoming = CosineSampleHemisphere(sample);
		if (incoming.Y() < 0.f) { incoming.Y() *= -1.f; }
		pdf = Pdf(outgoing, incoming);
		
		return Evaluate(outgoing, incoming);
	}

	virtual Color Reflectance(const Vector& outgoing, uint32_t sampleCount, const std::pair<float, float>* samples) const = 0;
	
	virtual float Pdf(const Vector& outgoing, const Vector& incoming) const
	{
		return SameHemisphere(outgoing, incoming) ? 0.f : AbsCos(incoming) * InversePi;
	}

protected:
	Type m_Type;
};
