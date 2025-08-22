﻿// By hzFishy 2025 - Do whatever you want with it

#pragma once

#include "BlueprintComponentReference.h"
#include "FUPickers.generated.h"


/* Copy of EComponentSocketType */
UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EFUEComponentSocketType: uint8
{
	/** Not a valid socket or bone name. */
	Invalid		  = 0 UMETA(Hidden),
	/** Skeletal bone. */
	Bone			= 1 << 0,
	/** Socket. */
	Socket			= 1 << 1,
};
ENUM_CLASS_FLAGS(EFUEComponentSocketType);


namespace FU_Pickers
{
	FISHYUTILS_API EFUEComponentSocketType ConvertSocketTypeFromEngine(EComponentSocketType::Type Type);
}


USTRUCT(BlueprintType, DisplayName = "Socket Picker")
struct FISHYUTILS_API FFUPickerSocket
{
	GENERATED_BODY();

	FFUPickerSocket();
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FName SelectedSocket = NAME_None;

#if WITH_EDITORONLY_DATA
	
	UPROPERTY(EditAnywhere, meta=(AllowedClasses="/Script/Engine.PrimitiveComponent"))
	FBlueprintComponentReference ComponentReference;
	
	UPROPERTY(EditAnywhere)
	FName ComponentName = NAME_None;
	
	UPROPERTY(EditAnywhere, meta = (Bitmask, BitmaskEnum = "/Script/FishyUtils.EFUEComponentSocketType"))
	uint8 SocketFilterFlags = static_cast<uint8>(EFUEComponentSocketType::Bone);
#endif
};
