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

#include "Core/Math/Color.h"
#include "Core/Math/Vector.h"
#include "Core/Math/Sampling.h"
#include "Core/Components/Sampler.h"

inline float Cos(const Vector& w) { return w.Y(); }
inline float Cos2(const Vector& w) { return w.Y() * w.Y(); }
inline float AbsCos(const Vector& w) { return std::abs(w.Y()); }

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
	return (sin == 0.f) ? 0.f : w.Z() / sin;
}
inline float Sin2Phi(const Vector& w) { return SinPhi(w) * SinPhi(w); }

inline Vector Reflect(const Vector& wo, const Vector& n)
{
	return -wo + 2 * Dot(wo, n) * n;
}

inline bool Refract(const Vector& incoming, const Normal& normal, float eta, Vector& transmitted)
{
	float cosI = Dot(normal, incoming);
	float sin2I = std::max(0.f, 1.f - cosI * cosI);
	float sin2T = eta * eta * sin2I;
	if (sin2T >= 1.f) { return false; }
	
	float cosT = std::sqrt(1 - sin2T);
	
	transmitted = eta * -incoming + (eta * cosI - cosT) * Vector(normal);
	return true;
}

inline bool SameHemisphere(const Vector& w, const Vector& wp) { return w.Y() * wp.Y() > 0; }
inline bool SameHemisphere(const Vector& w, const Normal& wp) { return w.Y() * wp.Y() > 0; }

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
	
	Type GetType() const { return m_Type; }

	virtual Color Evaluate(const Vector& outgoing, const Vector& incoming) const = 0;
	virtual Color EvaluateSample(const Vector& outgoing, Vector& incoming, Sampler* sampler, float& pdf) const
	{
		std::pair<float, float> sample = sampler->Get2D();

		incoming = CosineSampleHemisphere(sample);
		incoming.Y() = std::abs(incoming.Y());
		pdf = Pdf(outgoing, incoming);
		
		return Evaluate(outgoing, incoming);
	}
	
	virtual float Pdf(const Vector& outgoing, const Vector& incoming) const
	{
		return SameHemisphere(outgoing, incoming) ? AbsCos(incoming) * InversePi : 0.f;
	}

protected:
	Type m_Type;
};
