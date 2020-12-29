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

float PowerHeuristic(int nf, float fPdf, int ng, float gPdf)
{
	float f = nf * fPdf, g = ng * gPdf;
	return (f * f) / (f * f + g * g);
}
