// By hzFishy 2025 - Do whatever you want with it

#include "Classes/Engine/FUStaticMeshComponent.h"


UFUStaticMeshComponent::UFUStaticMeshComponent(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	Mobility = EComponentMobility::Static;
	SetGenerateOverlapEvents(false);
}
