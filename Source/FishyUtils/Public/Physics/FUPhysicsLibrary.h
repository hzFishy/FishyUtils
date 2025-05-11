// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FUPhysicsLibrary.generated.h"


USTRUCT(BlueprintType)
struct FFUSkeletalOverlapResult
{
	GENERATED_BODY()

	FBodyInstance* BodyInstance = nullptr;
	UPROPERTY(BlueprintReadOnly) int32 InstanceBoneIndex = -1;
};

USTRUCT(BlueprintType)
struct FFUSkeletalOverlapDebugParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere) bool bDebug = false;
	UPROPERTY(EditAnywhere) float Time = 5;
	UPROPERTY(EditAnywhere) float Thickness = 2;
	UPROPERTY(EditAnywhere) float DepthPriority = 0;
};


/**
 * 
 */
UCLASS()
class FISHYUTILS_API UFUPhysicsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * check if any skeletal BI can overlap with primitive BI.
	 * we can have multiple skeletal BI overlapping with this primitive BI,
	 * so the user should store previous overlapping BIs to get the new one (if called in a beginoverlap we should only find 1 new)
	 */
	UFUNCTION(BlueprintCallable)
	static void FindSkeletalOverlappingBodies(UPrimitiveComponent* OverlappingPrimitiveComponent, USkeletalMeshComponent* OverlappingSkeletalMeshComponent, TArray<FFUSkeletalOverlapResult>& OutOverlapResults, const FFUSkeletalOverlapDebugParams& DebugParams = FFUSkeletalOverlapDebugParams());
};
