// By hzFishy - 2026 - Do whatever you want with it.


#include "Utility/FUUILibrary.h"

#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"


void UFUUILibrary::SetInputModeAndMouseVisibility(APlayerController* PlayerController, EFUInputMode NewInputMode,
	EFUNewMouseVisibility NewMouseVisibility, bool bResetCursorToCenter)
{
	if (!IsValid(PlayerController)) { return; }
	
	switch (NewInputMode)
	{
		case EFUInputMode::DontChange:
			{
					
			}
			break;
		case EFUInputMode::GameOnly:
			{
				FInputModeGameOnly InputMode = FInputModeGameOnly();
				PlayerController->SetInputMode(InputMode);
			}
			break;
		case EFUInputMode::UIOnly:
			{
				FInputModeUIOnly InputMode = FInputModeUIOnly();
				PlayerController->SetInputMode(InputMode);
			}
			break;
		case EFUInputMode::GameAndUI:
			{
				FInputModeGameAndUI InputMode = FInputModeGameAndUI();
				PlayerController->SetInputMode(InputMode);
			}
			break;
	}
	
	switch (NewMouseVisibility)
	{
		case EFUNewMouseVisibility::DontChange:
			{
						
			}
			break;
		case EFUNewMouseVisibility::Show:
			{
				PlayerController->SetShowMouseCursor(true);
			}
			break;
		case EFUNewMouseVisibility::Hide:
			{
				PlayerController->SetShowMouseCursor(false);
			}
			break;
	}

	if (bResetCursorToCenter)
	{
		FVector2D Size = UWidgetLayoutLibrary::GetViewportSize(PlayerController);
		int32 X = FMath::TruncToInt(Size.X / 2);
		int32 Y = FMath::TruncToInt(Size.Y / 2);
		PlayerController->SetMouseLocation(X, Y);
	}
}

void UFUUILibrary::SetMouseVisibility(APlayerController* PlayerController, EFUNewMouseVisibility NewMouseVisibility)
{
	if (!IsValid(PlayerController)) { return; }
	
	switch (NewMouseVisibility)
	{
	case EFUNewMouseVisibility::DontChange:
		{
			
		}
		break;
	case EFUNewMouseVisibility::Show:
		{
			PlayerController->SetShowMouseCursor(true);
		}
		break;
	case EFUNewMouseVisibility::Hide:
		{
			PlayerController->SetShowMouseCursor(false);
		}
		break;
	}
}

void UFUUILibrary::ResetMouseToCenter(APlayerController* PlayerController)
{
	if (!IsValid(PlayerController)) { return; }
	
	FVector2D Size = UWidgetLayoutLibrary::GetViewportSize(PlayerController);
	int32 X = FMath::TruncToInt(Size.X / 2);
	int32 Y = FMath::TruncToInt(Size.Y / 2);
	PlayerController->SetMouseLocation(X, Y);
}

void UFUUILibrary::SetUIOnlyInputMode(APlayerController* PlayerController, EMouseLockMode LockMouse, UUserWidget* WidgetToFocus)
{
	if (!IsValid(PlayerController)) { return; }
	
	FInputModeUIOnly InputMode = FInputModeUIOnly();
	InputMode.SetLockMouseToViewportBehavior(LockMouse);
	if (IsValid(WidgetToFocus))
	{
		InputMode.SetWidgetToFocus(WidgetToFocus->GetCachedWidget());
	}
	PlayerController->SetInputMode(InputMode);
}

void UFUUILibrary::SetGameOnlyInputMode(APlayerController* PlayerController, bool bConsumeCaptureMouseDown)
{
	if (!IsValid(PlayerController)) { return; }
	
	FInputModeGameOnly InputMode = FInputModeGameOnly();
	InputMode.SetConsumeCaptureMouseDown(bConsumeCaptureMouseDown);
	PlayerController->SetInputMode(InputMode);
}

void UFUUILibrary::SetGameAndUIInputMode(APlayerController* PlayerController, bool bHideCursorDuringCapture, EMouseLockMode LockMouse, UUserWidget* WidgetToFocus)
{
	if (!IsValid(PlayerController)) { return; }
	
	FInputModeGameAndUI InputMode = FInputModeGameAndUI();
	InputMode.SetHideCursorDuringCapture(bHideCursorDuringCapture);
	InputMode.SetLockMouseToViewportBehavior(LockMouse);
	if (IsValid(WidgetToFocus))
	{
		InputMode.SetWidgetToFocus(WidgetToFocus->GetCachedWidget());
	}
	PlayerController->SetInputMode(InputMode);
}

bool UFUUILibrary::AreWidgetsOverlapping(UWidget* WidgetA, UWidget* WidgetB)
{
	const auto& WidgetAGeometry = WidgetA->GetCachedGeometry();
	const auto& WidgetBGeometry = WidgetB->GetCachedGeometry();
	
	// here we get the Top Left pixel position
	FVector2D WidgetAPixelPosition;
	FVector2D WidgetAViewportPosition;
	USlateBlueprintLibrary::LocalToViewport(WidgetA, WidgetAGeometry, FVector2D(0, 0), WidgetAPixelPosition, WidgetAViewportPosition);
	
	FVector2D WidgetBPixelPosition;
	FVector2D WidgetBViewportPosition;
	USlateBlueprintLibrary::LocalToViewport(WidgetB, WidgetBGeometry, FVector2D(0, 0), WidgetBPixelPosition, WidgetBViewportPosition);
	
	auto WidgetASize = WidgetAGeometry.GetAbsoluteSize();
	auto WidgetBSize = WidgetBGeometry.GetAbsoluteSize();
	
	bool bWidgetALeftUnderBRight = WidgetAPixelPosition.X < (WidgetBPixelPosition.X + WidgetBSize.X);
	bool bWidgetARightOverBLeft = (WidgetAPixelPosition.X + WidgetASize.X) > WidgetBPixelPosition.X;
	bool bWidgetATopUnderBBottom = WidgetAPixelPosition.Y < (WidgetBPixelPosition.Y + WidgetBSize.Y);
	bool bWidgetABottomOverBTop = (WidgetAPixelPosition.Y + WidgetASize.Y) > WidgetBPixelPosition.Y;
	
	return bWidgetALeftUnderBRight && bWidgetARightOverBLeft && bWidgetATopUnderBBottom && bWidgetABottomOverBTop;
}
