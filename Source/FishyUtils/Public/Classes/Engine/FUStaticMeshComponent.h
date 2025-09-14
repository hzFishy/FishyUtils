// By hzFishy 2025 - Do whatever you want with it

#pragma once

#include "FUStaticMeshComponent.generated.h"


/**
 * Fishy Utils base class for static mesh components
 */
UCLASS()
class FISHYUTILS_API UFUStaticMeshComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	UFUStaticMeshComponent(const FObjectInitializer& ObjectInitializer);
};
