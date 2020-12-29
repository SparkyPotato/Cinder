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
#include "Core/Math/Math.h"

float Lerp(float from, float to, float ratio)
{
	return from + (to - from) * ratio;
}

bool IsNearlyEqual(float value, float equalTo, float tolerance)
{
	return value < equalTo + tolerance && value > equalTo - tolerance;
}

bool SolveQuadratic(float coA, float coB, float coC, float& outA, float& outB)
{
	double a(coA);
	double b(coB);
	double c(coC);

	double discriminant = b * b - 4 * a * c;
	if (discriminant < 0) { return false; }

	double rootDisc = std::sqrt(discriminant);
	double quotient;
	if (b < 0) { quotient = -0.5 * (b - rootDisc); }
	else { quotient = -0.5 * (b + rootDisc); }

	double solA = quotient / a;
	double solB = c / quotient;

	outA = float(std::min(solA, solB));
	outB = float(std::max(solA, solB));
	return true;
}

bool TestQuadratic(float coA, float coB, float coC)
{
	double a(coA);
	double b(coB);
	double c(coC);

	double discriminant = b * b - 4 * a * c;
	if (discriminant < 0) { return false; }
	else { return true; }
}
