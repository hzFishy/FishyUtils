// By hzFishy - 2026 - Do whatever you want with it.

#include "Classes/Engine/FUStaticMeshComponent.h"


UFUStaticMeshComponent::UFUStaticMeshComponent(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	Mobility = EComponentMobility::Static;
	SetGenerateOverlapEvents(false);
}
