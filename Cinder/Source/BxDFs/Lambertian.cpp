#include "PCH.h"
#include "BxDFs/Lambertian.h"

LambertianBRDF::LambertianBRDF(const Color& base)
	: BxDF(Type(Reflection | Diffuse)), m_Base(base)
{}

Color LambertianBRDF::Evaluate(const Vector& outgoing, const Vector& incoming) const
{
	return m_Base * InversePi;
}
