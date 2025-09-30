// By hzFishy 2025 - Do whatever you want with it


#include "Utility/FUUILibrary.h"

#include "Blueprint/WidgetLayoutLibrary.h"

void UFUUILibrary::SetInputModeAndMouseVisibility(APlayerController* PlayerController, EFUInputMode NewInputMode,
	EFUNewMouseVisibility NewMouseVisibility, bool bResetCursorToCenter)
{
	if (!IsValid(PlayerController))
	{
		return;
	}
	
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
