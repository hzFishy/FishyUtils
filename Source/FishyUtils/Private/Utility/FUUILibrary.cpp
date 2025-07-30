// By hzFishy 2025 - Do whatever you want with it


#include "Utility/FUUILibrary.h"

void UFUUILibrary::SetInputModeAndMouseVisibility(APlayerController* PlayerController, EFUInputMode NewInputMode, EFUNewMouseVisibility NewMouseVisibility)
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
}
