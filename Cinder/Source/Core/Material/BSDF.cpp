#include "PCH.h"
#include "Core/Material/BSDF.h"

BSDF::BSDF(const RayIntersection& intersection)
{

}

void BSDF::Add(BxDF* bxdf)
{

}

uint16_t BSDF::Components(BxDF::Type type /*= BxDF::All*/) const
{

}

Vector BSDF::ToLocal(const Vector& vector) const
{

}

Vector BSDF::ToWorld(const Vector& vector) const
{

}

Color BSDF::Evaluate(const Vector& outgoing, const Vector& incoming, BxDF::Type type /*= BxDF::All*/) const
{

}

Color BSDF::EvaluateSample(const Vector& outgoing, Vector& incoming, const std::pair<float, float>& sample, float& pdf, BxDF::Type type /*= BxDF::All*/) const
{

}

Color BSDF::Reflectance(const Vector& outgoing, uint32_t sampleCount, const std::pair<float, float>* samples, BxDF::Type type /*= BxDF::All*/) const
{

}

float BSDF::Pdf(const Vector& outgoing, const Vector& incoming, BxDF::Type type /*= BxDF::All*/) const
{

}
