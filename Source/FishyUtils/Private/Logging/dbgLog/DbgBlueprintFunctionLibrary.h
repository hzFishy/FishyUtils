// Copyright ©  Jack Holland.
//
// DbgLOG is free software. It comes without any warranty, to the extent permitted
// by applicable law. You can redistribute it and/or modify it under the terms
// of the Do What The Fuck You Want To Public License, Version 2, as published
// by Sam Hocevar:
//
//            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
//                    Version 2, December 2004
//
// Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>
//
// Everyone is permitted to copy and distribute verbatim or modified
// copies of this license document, and changing it is allowed as long
// as the name is changed.
//
//            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
//   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//
//  0. You just DO WHAT THE FUCK YOU WANT TO.
#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

// Relative path is assumed, must update if you move the files around.
#include "./dbgLog.h"
/*#include "DbgBlueprintFunctionLibrary.generated.h"



USTRUCT(BlueprintType)
struct FDbgLogArgs
{
	GENERATED_BODY()
public:
	// If not set will be determined by the log verbosity
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor ScreenColor = FColor::Transparent;

	// If negative it will be determined by the log verbosity
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ScreenDuration = -1.f;

	// If set to anything other than -1 it will be used as the key for the screen log.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ScreenKey = -1;

	// If true the log is prefixed with the netmode and PIE instance number.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bPrefixPIEInstanceInfo = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bLogDateAndTime = false;

	// If true the log will show up as a slate notification pop-up in the bottom right.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bLogToSlateNotify = false;

	// If we are outputting to a slate notify, should that be the only place we output?
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bOnlyLogToSlateNotify = false;

	// Logs a message to the editor message log (this is a window that pops up showing you any errors or warnings)
	// You would see this when accessing null in a BP graph for example after you quit PIE.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bLogToEditorMessageLog = false;

	// If true we instantly spawn the editor message log window, otherwise you need to manually check the window.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(EditCondition="bLogToEditorMessageLog"))
	bool bShouldShowEditorMessageLogImmediately = false;
};



UENUM(BlueprintType)
enum struct EDbgLogVerbosity : uint8
{
	Display=0,
	Warning,
	Error,
};



UCLASS()
class SANDBOX_API UDbgBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, meta=( DevelopmentOnly, WorldContext="WorldContextObject",
		LogCategory="Log", LogMessage="Something", LogOutput="Both", LogVerbosity="Display",
		Keywords="Log,Print,Console,Output,bf,dbg,debug" ) )
	static void OutputLog( UObject* WorldContextObject,
		FName LogCategory,
		FString LogMessage,
		EDbgLogOutput LogOutput,
		EDbgLogVerbosity LogVerbosity,
		FDbgLogArgs LogExtraArgs
		);
	

	
};
*/
