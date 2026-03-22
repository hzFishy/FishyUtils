// By hzFishy - 2026 - Do whatever you want with it.

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
	UFUNCTION(BlueprintCallable, Category="FishyUtils|Utility|UI", meta=(DefaultToSelf, HideSelfPin))
	static void SetInputModeAndMouseVisibility(APlayerController* PlayerController, EFUInputMode NewInputMode,
		EFUNewMouseVisibility NewMouseVisibility, bool bResetCursorToCenter);
	
	UFUNCTION(BlueprintCallable, Category="FishyUtils|Utility|UI", meta=(DefaultToSelf, HideSelfPin))
	static void SetMouseVisibility(APlayerController* PlayerController, EFUNewMouseVisibility NewMouseVisibility);
	
	UFUNCTION(BlueprintCallable, Category="FishyUtils|Utility|UI", meta=(DefaultToSelf, HideSelfPin))
	static void ResetMouseToCenter(APlayerController* PlayerController);
	
	UFUNCTION(BlueprintCallable, Category="FishyUtils|Utility|UI", meta=(DefaultToSelf, HideSelfPin))
	static void SetUIOnlyInputMode(APlayerController* PlayerController, EMouseLockMode LockMouse = EMouseLockMode::LockInFullscreen, UUserWidget* WidgetToFocus = nullptr);
	
	UFUNCTION(BlueprintCallable, Category="FishyUtils|Utility|UI", meta=(DefaultToSelf, HideSelfPin))
	static void SetGameOnlyInputMode(APlayerController* PlayerController, bool bConsumeCaptureMouseDown = true);
	
	UFUNCTION(BlueprintCallable, Category="FishyUtils|Utility|UI", meta=(DefaultToSelf, HideSelfPin))
	static void SetGameAndUIInputMode(APlayerController* PlayerController, bool bHideCursorDuringCapture = true, EMouseLockMode LockMouse = EMouseLockMode::DoNotLock, UUserWidget* WidgetToFocus = nullptr);
};
