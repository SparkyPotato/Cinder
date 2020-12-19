#pragma once

#include "Core/Math/Vector.h"

Point ConcentricSampleDisk(const std::pair<float, float>& sample);

Vector CosineSampleHemisphere(const std::pair<float, float>& sample);
float CosineHemispherePdf(float cos);

float PowerHeuristic(int nf, float fPdf, int ng, float gPdf);
