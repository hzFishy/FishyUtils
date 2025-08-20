// By hzFishy 2025 - Do whatever you want with it

#pragma once

// use FU_WITH_CONSOLE to control when we want to have access to console namespace
#if UE_BUILD_SHIPPING || NO_CVARS
	#define FU_WITH_CONSOLE 0
#else
	#define FU_WITH_CONSOLE 1
#endif

namespace FU_Console
{
#if FU_WITH_CONSOLE
	
	/**
	 *  @return "ProjectPrefix.Catepgry.CommandName"
	 */
	static FString BuildFullCommandString(const FString& Category, const FString& CommandName)
	{
		return FString::Printf(TEXT("%s.%s"), *Category, *CommandName);
	};

	/* No world, no args */
	struct FFUAutoConsoleCommand : private FAutoConsoleCommand
	{
		FFUAutoConsoleCommand(const FString& Category, const FString& CommandName, const FString& HelpDescription, const FConsoleCommandDelegate& Delegate):
			FAutoConsoleCommand(*BuildFullCommandString(Category, CommandName), *HelpDescription, Delegate, ECVF_Default)
		{}
	};
	
	/* No world, with args */
	struct FFUAutoConsoleCommandWithArgs : private FAutoConsoleCommand
	{
		FFUAutoConsoleCommandWithArgs(const FString& Category, const FString& CommandName, const FString& HelpDescription, const FConsoleCommandWithArgsDelegate& Delegate):
			FAutoConsoleCommand(*BuildFullCommandString(Category, CommandName), *HelpDescription, Delegate, ECVF_Default)
		{}
	};
	
	/* World, no args */
	struct FFUAutoConsoleCommandWithWorld : private FAutoConsoleCommandWithWorld
	{
		FFUAutoConsoleCommandWithWorld(const FString& Category, const FString& CommandName, const FString& HelpDescription, const FConsoleCommandWithWorldDelegate& Delegate):
			FAutoConsoleCommandWithWorld(*BuildFullCommandString(Category, CommandName), *HelpDescription, Delegate, ECVF_Default)
		{}
	};
	
	/* World and with args */
	struct FFUAutoConsoleCommandWithWorldAndArgs : private FAutoConsoleCommandWithWorldAndArgs
	{
		FFUAutoConsoleCommandWithWorldAndArgs(const FString& Category, const FString& CommandName, const FString& HelpDescription, const FConsoleCommandWithWorldAndArgsDelegate& Delegate):
			FAutoConsoleCommandWithWorldAndArgs(*BuildFullCommandString(Category, CommandName), *HelpDescription, Delegate, ECVF_Default)
		{}
	};

#define FU_CMD_BOOL_WITH_OPT_FLOAT_CPPONLY(Id, Cmd, CmdHelp, BoolVar, FloatVar, DefaultFloatVarValue) \
		static bool BoolVar = false; \
		static float FloatVar = DefaultFloatVarValue; \
		FU_Console::FFUAutoConsoleCommandWithArgs C##CallbackFunc("", Cmd, CmdHelp, \
			FConsoleCommandWithArgsDelegate::CreateLambda([](const TArray<FString>& Args) \
			{ \
				/* if no time arg, toggle */ \
				/* if enabled and time arg, keep enabled and change time */ \
				const float Newtime = !Args.IsEmpty() ? FCString::Atof(*Args[0]) : DefaultFloatVarValue; \
				if (!BoolVar) \
				{ \
					BoolVar = true; FloatVar = Newtime; \
				} \
				else \
				{ \
					if (Args.IsEmpty()) \
					{ \
						BoolVar = false; FloatVar = Newtime; \
					} \
					else \
					{ \
						BoolVar = true; FloatVar = Newtime; \
					} \
				} \
			}) \
		); \

#endif
}
