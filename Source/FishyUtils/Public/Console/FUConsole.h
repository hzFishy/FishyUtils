// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// use FU_WITH_CONSOLE to control when we want to have access to console namespace
#if UE_BUILD_SHIPPING || NO_CVARS
	#define FU_WITH_CONSOLE 0
#else
	#define FU_WITH_CONSOLE 1
#endif

#ifndef FU_CONSOLE_PROJECTNAME
	// edit this in your Build.cs file using PublicDefinitions.Add("FU_CONSOLE_PROJECTNAME=\"PROJECTNAME\"");
	#define FU_CONSOLE_PROJECTNAME "FU"
#endif

namespace FU_Console
{

#if FU_WITH_CONSOLE
	
	/**
	 *  @return "ProjectPrefix.Catepgry.CommandName"
	 */
	static FString BuildFullCommandString(const FString& Category, const FString& CommandName)
	{
		return FString::Printf(TEXT("%s.%s.%s"), TEXT(FU_CONSOLE_PROJECTNAME), *Category, *CommandName);
	};

	/* No world, no args */
	struct FFUAutoConsoleCommand : private FAutoConsoleCommand
	{
		FFUAutoConsoleCommand(const FString& Category, const FString& CommandName,
			const FString& HelpDescription, const FConsoleCommandDelegate& Delegate)
			: FAutoConsoleCommand(
				*BuildFullCommandString(Category, CommandName),
				*HelpDescription, Delegate,
				ECVF_Default)
		{}
	};
	
	/* No world, with args */
	struct FFUAutoConsoleCommandWithArgs : private FAutoConsoleCommand
	{
		FFUAutoConsoleCommandWithArgs(const FString& Category, const FString& CommandName,
			const FString& HelpDescription, const FConsoleCommandWithArgsDelegate& Delegate)
			: FAutoConsoleCommand(
				*BuildFullCommandString(Category, CommandName),
				*HelpDescription, Delegate,
				ECVF_Default)
		{}
	};
	
	/* World, no args */
	struct FFUAutoConsoleCommandWithWorld : private FAutoConsoleCommandWithWorld
	{
		FFUAutoConsoleCommandWithWorld(const FString& Category, const FString& CommandName,
			const FString& HelpDescription, const FConsoleCommandWithWorldDelegate& Delegate)
			: FAutoConsoleCommandWithWorld(
				*BuildFullCommandString(Category, CommandName),
				*HelpDescription, Delegate,
				ECVF_Default)
		{}
	};
	
	/* World and with args */
	struct FFUAutoConsoleCommandWithWorldAndArgs : private FAutoConsoleCommandWithWorldAndArgs
	{
		FFUAutoConsoleCommandWithWorldAndArgs(const FString& Category, const FString& CommandName,
			const FString& HelpDescription, const FConsoleCommandWithWorldAndArgsDelegate& Delegate)
			: FAutoConsoleCommandWithWorldAndArgs(
				*BuildFullCommandString(Category, CommandName),
				*HelpDescription, Delegate,
				ECVF_Default)
		{}
	};
	
#endif
	
}