// By hzFishy 2025 - Do whatever you want with it

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "FUDeveloperSettings.generated.h"


/**
 * Subclass this in your project
 * Code:
 * <code>
 * // .h
 * UCLASS(Config=PushThemAll, Abstract)
 * class PUSHTHEMALL_API UPTADeveloperSettings : public UFUDeveloperSettings
 * // .cpp
 * UPTADeveloperSettings::UPTADeveloperSettings() : UFUDeveloperSettings("PushThemAll")
 * {}
 * </code>
 *
 * Then you can make whatever subclass you want using the class you just made.
 * Example:
 * <code>
 * UCLASS(DisplayName="Equipment System")
 * class PUSHTHEMALL_API UPTAEquipmentDeveloperSettings : public UPTADeveloperSettings
 * {
 * 	GENERATED_BODY()
 * 
 * public:
 * 	UPROPERTY(Config, EditAnywhere, Category = "UI")
 * 	TSoftClassPtr<UPTAEquipmentHotbarSlotWidget> HotbarSlotWidget = nullptr;
 * };
 * </code>
 */
UCLASS(defaultconfig, Abstract)
class FISHYUTILS_API UFUDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UFUDeveloperSettings() = default;
	UFUDeveloperSettings(FName GameName);

	// public:
	// 	UPROPERTY(Config, EditAnywhere, Category = "")
};
