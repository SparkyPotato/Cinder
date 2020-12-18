#include "PCH.h"
#include "Core/Math/Sampling.h"

Point ConcentricSampleDisk(const std::pair<float, float>& sample)
{
	Point uOffset = { 2.f * sample.first - 1.f, 0.f, 2.f * sample.second - 1.f };

	if (uOffset.X() == 0 && uOffset.Z() == 0) { return Point(); }

	float theta, r;
	if (std::abs(uOffset.X()) > std::abs(uOffset.Z())) 
	{
		r = uOffset.X();
		theta = Pi * (uOffset.Z() / uOffset.X()) / 4.f;
	}
	else
	{
		r = uOffset.Z();
		theta = Pi / 2.f - Pi * (uOffset.X() / uOffset.Z()) / 4.f;
	}

	return Point(r * std::cos(theta), 0.f, r * std::sin(theta));
}

Vector CosineSampleHemisphere(const std::pair<float, float>& sample)
{
	Point d = ConcentricSampleDisk(sample);
	float y = std::sqrt(std::max(0.f, 1.f - d.X() * d.X() - d.Z() * d.Z()));
	return Vector(d.X(), y, d.Z());
}

float CosineHemispherePdf(float cos)
{
	return cos * InversePi;
}

Vector UniformSampleSphere(const std::pair<float, float>& sample)
{
	float z = 1.f - 2.f * sample.first;
	float r = std::sqrt(std::max(0.f, 1.f - z * z));
	float phi = 2 * Pi * sample.second;
	return Vector(r * std::cos(phi), r * std::sin(phi), z);
}
