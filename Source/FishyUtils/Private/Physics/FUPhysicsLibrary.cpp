// Fill out your copyright notice in the Description page of Project Settings.


#include "Physics/FUPhysicsLibrary.h"

#include "Draw/FUDraw.h"

void UFUPhysicsLibrary::FindSkeletalOverlappingBodies(UPrimitiveComponent* OverlappingPrimitiveComponent, USkeletalMeshComponent* OverlappingSkeletalMeshComponent,
                                                      TArray<FFUSkeletalOverlapResult>& OutOverlapResults, const FFUSkeletalOverlapDebugParams& DebugParams)
{
	if (!IsValid(OverlappingPrimitiveComponent) || !IsValid(OverlappingSkeletalMeshComponent)) { return; }

	FBodyInstance* PrimitiveBI = OverlappingPrimitiveComponent->GetBodyInstance();
	const TArray<FBodyInstance*> SkeletalBIs = OverlappingSkeletalMeshComponent->Bodies;

	// check if any skeletal BI can overlap with primitive BI
	// we can have multiple skeletal BI overlapping with this primitive BI,
	// so the user should store previous overlapping BIs to get the new one (if called in a beginoverlap we should only find 1 new)

	for (auto& SkeletalBI : SkeletalBIs)
	{
		const FTransform BodyTransform = SkeletalBI->GetUnrealWorldTransform();
		const bool bOverlaps = SkeletalBI->OverlapTestForBody(
			BodyTransform.GetLocation(),
			BodyTransform.GetRotation(),
			PrimitiveBI,
			false
		);

		if (DebugParams.bDebug)
		{
			FU_Draw::DrawDebugBodyInstance(
			   OverlappingPrimitiveComponent->GetWorld(),
			   *SkeletalBI,
			   bOverlaps ? FColor::Red : FColor::White,
			   DebugParams.Time,
			   DebugParams.Thickness,
			   DebugParams.DepthPriority
		   );
		}

		if (bOverlaps)
		{
			OutOverlapResults.Emplace(FFUSkeletalOverlapResult(SkeletalBI, SkeletalBI->InstanceBoneIndex));
		}
	}
}
