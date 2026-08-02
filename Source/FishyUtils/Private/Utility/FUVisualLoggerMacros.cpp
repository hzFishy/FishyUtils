// By hzFishy - 2026 - Do whatever you want with it.


#include "Utility/FUVisualLoggerMacros.h"
#include "Components/CapsuleComponent.h"


float FU::VisualLogger::GetCapsuleHalfHeight(const UCapsuleComponent* Capsule)
{
	return Capsule->GetScaledCapsuleHalfHeight();
}

float FU::VisualLogger::GetCapsuleRadius(const UCapsuleComponent* Capsule)
{
	return Capsule->GetScaledCapsuleRadius();
}

FVector FU::VisualLogger::GetCapsuleLocation(const UCapsuleComponent* Capsule)
{
	return Capsule->GetComponentLocation();
}

FQuat FU::VisualLogger::GetCapsuleRotation(const UCapsuleComponent* Capsule)
{
	return Capsule->GetComponentRotation().Quaternion();
}
