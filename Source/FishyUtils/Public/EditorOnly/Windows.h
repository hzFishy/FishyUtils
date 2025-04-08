// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GenericPlatform/GenericPlatformMisc.h"


namespace FU_Windows::MessageDialog
{
	FISHYUTILS_API void Open(const TFunction<void(EAppReturnType::Type UserResponse)>& CallbackResponse, EAppMsgCategory MessageCategory,
		EAppMsgType::Type MessageType, const FString& Message, const FString& Title);
}
