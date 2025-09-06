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
		if (Category.IsEmpty())
		{
			return CommandName;
		}
		else if (CommandName.IsEmpty())
		{
			return Category;
		}
		else
		{
			return FString::Printf(TEXT("%s.%s"), *Category, *CommandName);
		}
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
		FU_Console::FFUAutoConsoleCommandWithArgs C##Id("", Cmd, CmdHelp, \
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

	
	/** Same as FU_CMD_BOOL_WITH_OPT_FLOAT_CPPONLY but BoolVar and FloatVar are expected to be vars marked as extern in the header */
#define FU_CMD_BOOL_WITH_OPT_FLOAT_CPP_WITH_EXTERN(Id, Cmd, CmdHelp, BoolVar, FloatVar, DefaultFloatVarValue) \
		bool BoolVar = false; \
		float FloatVar = DefaultFloatVarValue; \
		FU_Console::FFUAutoConsoleCommandWithArgs C##Id("", Cmd, CmdHelp, \
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

/** On each call the bool var will flip */
#define FU_CMD_BOOL_TOGGLE(Id, Cmd, CmdHelp, BoolVar, DefaultBoolVal) \
    static bool BoolVar = DefaultBoolVal; \
    FU_Console::FFUAutoConsoleCommand C##Id("", Cmd, CmdHelp, \
        FConsoleCommandDelegate::CreateLambda([]() \
        { \
            BoolVar = !BoolVar; \
        }) \
    ); \

/** Get first instance of given actor class in editor/game world and run given function with optional args */
#define FU_CMD_ACTOR_SINGLERUNFUNC(Id, Cmd, CmdHelp, ActorClass, FuncName, ...) \
    FU_Console::FFUAutoConsoleCommand C##Id("", Cmd, CmdHelp, \
        FConsoleCommandDelegate::CreateLambda([]() \
        { \
			for (auto It = TActorIterator<ActorClass>(GWorld); It; ++It) \
			{ \
				if (IsValid(*It)) \
				{ \
					It->FuncName(__VA_ARGS__); \
					break; \
				} \
			} \
        }) \
    ); \

/** Get all instances of given actor class in editor/game world and run given function with optional args */
#define FU_CMD_ACTOR_ALLRUNFUNC(Id, Cmd, CmdHelp, ActorClass, FuncName, ...) \
    FU_Console::FFUAutoConsoleCommand C##Id("", Cmd, CmdHelp, \
        FConsoleCommandDelegate::CreateLambda([]() \
        { \
			for (auto It = TActorIterator<ActorClass>(GWorld); It; ++It) \
			{ \
				if (IsValid(*It)) \
				{ \
					It->FuncName(__VA_ARGS__); \
				} \
			} \
        }) \
    ); \

/** Get first instance of given UObject class in editor/game world (excludes COD and Archetypes) and run given function with optional args */
#define FU_CMD_OBJECT_SINGLERUNFUNC(Id, Cmd, CmdHelp, ObjectClass, FuncName, ...) \
    FU_Console::FFUAutoConsoleCommand C##Id("", Cmd, CmdHelp, \
        FConsoleCommandDelegate::CreateLambda([]() \
        { \
			for (auto It = TObjectIterator<ObjectClass>(EObjectFlags::RF_ClassDefaultObject | EObjectFlags::RF_ArchetypeObject); It; ++It) \
			{ \
				if (IsValid(*It) && It->GetWorld() == GWorld) \
				{ \
					It->FuncName(__VA_ARGS__); \
					break; \
				} \
			} \
        }) \
    ); \

/** Get first instance of given UObject class in editor/game world (excludes COD and Archetypes) and run given function with optional args */
#define FU_CMD_OBJECT_ALLRUNFUNC(Id, Cmd, CmdHelp, ObjectClass, FuncName, ...) \
    FU_Console::FFUAutoConsoleCommand C##Id("", Cmd, CmdHelp, \
        FConsoleCommandDelegate::CreateLambda([]() \
        { \
			for (auto It = TObjectIterator<ObjectClass>(EObjectFlags::RF_ClassDefaultObject | EObjectFlags::RF_ArchetypeObject); It; ++It) \
			{ \
				if (IsValid(*It) && It->GetWorld() == GWorld) \
				{ \
					It->FuncName(__VA_ARGS__); \
				} \
			} \
        }) \
    ); \

#endif
}
