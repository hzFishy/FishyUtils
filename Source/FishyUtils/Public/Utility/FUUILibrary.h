﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FUUILibrary.generated.h"

UENUM(BlueprintType)
enum class EFUInputMode : uint8
{
	DontChange,
	GameOnly,
	UIOnly,
	GameAndUI
};

UENUM(BlueprintType)
enum class EFUNewMouseVisibility : uint8
{
	DontChange,
	Show,
	Hide,
};


/**
 * 
 */
UCLASS()
class FISHYUTILS_API UFUUILibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="FishyUtils|Utility|UI")
	static void SetInputModeAndMouseVisibility(APlayerController* PlayerController, EFUInputMode NewInputMode, EFUNewMouseVisibility NewMouseVisibility);
};
