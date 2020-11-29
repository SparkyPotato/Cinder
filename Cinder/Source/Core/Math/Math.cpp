#include "PCH.h"
#include "Math.h"

float Lerp(float from, float to, float ratio)
{
	return from + (to - from) * ratio;
}

float IsNearlyEqual(float value, float equalTo, float tolerance)
{
	return value <= equalTo + tolerance && value >= equalTo - tolerance;
}

bool SolveQuadratic(float coA, float coB, float coC, float& outA, float& outB)
{
	double a(coA);
	double b(coB);
	double c(coC);

	double discriminant = b * b - 4 * a * c;
	if (discriminant < 0) { return false; }
	if (discriminant == 0) { outA = outB = float(-b / 2 * a); return true; }

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
