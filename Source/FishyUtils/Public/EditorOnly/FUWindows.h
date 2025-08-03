// By hzFishy 2025 - Do whatever you want with it

#pragma once

#include "GenericPlatform/GenericPlatformMisc.h"


namespace FU::Windows::MessageDialog
{
	FISHYUTILS_API void Open(const TFunction<void(EAppReturnType::Type UserResponse)>& CallbackResponse, EAppMsgCategory MessageCategory,
		EAppMsgType::Type MessageType, const FString& Message, const FString& Title);
}
