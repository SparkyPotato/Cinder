#include "PCH.h"
#include "BxDFs/Lambertian.h"

LambertianBRDF::LambertianBRDF(const Color& base)
	: BxDF(BxDF::Type(BxDF::Reflection | BxDF::Diffuse)), m_Base(base)
{}

Color LambertianBRDF::Evaluate(const Vector& outgoing, const Vector& incoming) const
{
	return m_Base * InversePi;
}

Color LambertianBRDF::Reflectance(const Vector& outgoing, uint32_t sampleCount, const std::pair<float, float>* samples) const
{
	return m_Base;
}

