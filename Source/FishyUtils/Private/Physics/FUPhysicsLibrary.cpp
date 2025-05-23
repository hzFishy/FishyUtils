﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Physics/FUPhysicsLibrary.h"

#include "Draw/FUDraw.h"

void UFUPhysicsLibrary::FindSkeletalOverlappingBodies(UPrimitiveComponent* OverlappingPrimitiveComponent, USkeletalMeshComponent* OverlappingSkeletalMeshComponent,
                                                      TArray<FFUSkeletalOverlapResult>& OutOverlapResults, const FFUSkeletalOverlapDebugParams& DebugParams)
{
	if (!IsValid(OverlappingPrimitiveComponent) || !IsValid(OverlappingSkeletalMeshComponent)) { return; }

	FBodyInstance* PrimitiveBI = OverlappingPrimitiveComponent->GetBodyInstance();
	const TArray<FBodyInstance*> SkeletalBIs = OverlappingSkeletalMeshComponent->Bodies;

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
