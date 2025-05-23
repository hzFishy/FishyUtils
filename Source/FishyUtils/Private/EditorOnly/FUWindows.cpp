﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "EditorOnly/FUWindows.h"


void FU_Windows::MessageDialog::Open(const TFunction<void(EAppReturnType::Type UserResponse)>& CallbackResponse, EAppMsgCategory MessageCategory,
                                     EAppMsgType::Type MessageType, const FString& Message, const FString& Title)
{
	EAppReturnType::Type Response = FMessageDialog::Open(MessageCategory, MessageType, FText::FromString(Message), FText::FromString(Title));

	if (CallbackResponse)
	{
		CallbackResponse(Response);
	}
}
