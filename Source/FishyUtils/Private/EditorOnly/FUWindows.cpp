// By hzFishy 2025 - Do whatever you want with it


#include "EditorOnly/FUWindows.h"


void FU::Windows::MessageDialog::Open(const TFunction<void(EAppReturnType::Type UserResponse)>& CallbackResponse,
	EAppMsgCategory MessageCategory, EAppMsgType::Type MessageType, const FString& Message, const FString& Title)
{
	EAppReturnType::Type Response = FMessageDialog::Open(MessageCategory, MessageType, FText::FromString(Message), FText::FromString(Title));

	if (CallbackResponse)
	{
		CallbackResponse(Response);
	}
}
