#include "PCH.h"
#include "Core/Material/BSDF.h"

BSDF::BSDF(const Interaction& interaction)
	: m_GNormal(Vector(interaction.GNormal)), m_SNormal(Vector(interaction.SNormal))
{
	GenerateCoordinateSystem(m_SNormal, m_X, m_Z);
}

void BSDF::Add(BxDF* bxdf)
{
	ASSERT(m_BxDFCount + 1 < m_Max, "Too many BxDFs!");

	m_BxDFs[m_BxDFCount] = bxdf;
	m_BxDFCount++;

}

uint16_t BSDF::Components(BxDF::Type type) const
{
	uint16_t i = 0;
	for (uint16_t j = 0; j < m_BxDFCount; j++)
	{
		if (m_BxDFs[j]->IsType(type)) { i++; }
	}

	return i;
}

Vector BSDF::ToLocal(const Vector& vector) const
{
	return vector.TransformTo(m_X, m_SNormal, m_Z);
}

Vector BSDF::ToWorld(const Vector& vector) const
{
	return vector.TransformFrom(m_X, m_SNormal, m_Z);
}

Color BSDF::Evaluate(const Vector& outgoing, const Vector& incoming, BxDF::Type type) const
{
	Vector inc = ToLocal(incoming), out = ToLocal(outgoing);
	bool reflect = Dot(incoming, m_GNormal) * Dot(outgoing, m_GNormal) > 0;

	Color c;
	for (int i = 0; i < m_BxDFCount; ++i)
	{
		bool matchType = m_BxDFs[i]->IsType(type);
		bool matchReflect = reflect && (m_BxDFs[i]->GetType() & BxDF::Reflection);
		bool matchTransmit = !reflect && (m_BxDFs[i]->GetType() & BxDF::Transmission);

		if (matchType && (matchReflect || matchTransmit))
		{
			c += m_BxDFs[i]->Evaluate(out, inc);
		}
	}

	return c;
}

Color BSDF::EvaluateSample(const Vector& outgoing, Vector& incoming, const std::pair<float, float>& sample,
	float& pdf, BxDF::Type type, BxDF::Type* sampled) const
{
	uint16_t matching = Components(type);
	pdf = 0.f;
	if (matching == 0)
	{
		if (sampled) { *sampled = BxDF::Type(0); }
		return Color();
	}
	int comp = std::min(int(std::floor(sample.first * matching)), matching - 1);

	BxDF* bxdf = nullptr;
	int count = comp;
	for (int i = 0; i < m_BxDFCount; ++i)
	{
		if (m_BxDFs[i]->IsType(type) && count-- == 0)
		{
			bxdf = m_BxDFs[i];
			break;
		}
	}

	std::pair<float, float> remapped = { std::min(sample.first * matching - comp, 1.f), sample.second };

	Vector in, out = ToLocal(outgoing);
	if (out.Y() == 0.f) { return Color(); }

	if (sampled) { *sampled = bxdf->GetType(); }
	Color c = bxdf->EvaluateSample(out, in, remapped, pdf);
	if (pdf == 0)
	{
		if (sampled) { *sampled = BxDF::Type(0); }
		return 0.f;
	}
	incoming = ToWorld(in);

	if (!(bxdf->GetType() & BxDF::Specular) && matching > 1)
	{
		for (uint16_t i = 0; i < m_BxDFCount; i++)
		{
			if (m_BxDFs[i] != bxdf && m_BxDFs[i]->IsType(type))
			{
				pdf += m_BxDFs[i]->Pdf(out, in);
			}
		}
	}
	if (matching > 1) { pdf /= matching; }

	if (!(bxdf->GetType() & BxDF::Specular))
	{
		bool reflect = Dot(incoming, m_GNormal) * Dot(outgoing, m_GNormal) > 0;
		c = 0.f;
		for (int i = 0; i < m_BxDFCount; ++i)
		{
			bool matchReflect = reflect && (m_BxDFs[i]->GetType() & BxDF::Reflection);
			bool matchTransmit = !reflect && (m_BxDFs[i]->GetType() & BxDF::Transmission);

			if (m_BxDFs[i]->IsType(type) && (matchReflect || matchTransmit))
			{
				c += m_BxDFs[i]->Evaluate(out, in);
			}
		}
	}

	return c;
}

Color BSDF::Reflectance(const Vector& outgoing, uint32_t sampleCount, const std::pair<float, float>* samples, BxDF::Type type) const
{
	Vector o = ToLocal(outgoing);
	Color c;
	for (uint16_t i = 0; i < m_BxDFCount; i++)
	{
		if (m_BxDFs[i]->IsType(type))
		{
			c += m_BxDFs[i]->Reflectance(o, sampleCount, samples);
		}
	}

	return c;
}

float BSDF::Pdf(const Vector& outgoing, const Vector& incoming, BxDF::Type type) const
{
	Vector out = ToLocal(outgoing), in = ToLocal(incoming);
	if (out.Y() == 0.f) { return 0.f; }

	float pdf = 0.f;
	uint16_t matching = 0;
	for (uint16_t i = 0; i < m_BxDFCount; i++)
	{
		if (m_BxDFs[i]->IsType(type))
		{
			matching++;
			pdf += m_BxDFs[i]->Pdf(out, in);
		}
	}

	return matching > 0 ? pdf / matching : 0.f;
}
