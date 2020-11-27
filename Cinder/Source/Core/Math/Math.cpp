#include "PCH.h"

float Lerp(float from, float to, float ratio)
{
	return from + (to - from) * ratio;
}

float IsNearlyEqual(float value, float equalTo, float tolerance)
{
	return value <= equalTo + tolerance && value >= equalTo - tolerance;
}
