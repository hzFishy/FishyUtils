// By hzFishy - 2026 - Do whatever you want with it.

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

	/* No world, no args */
	struct FFUAutoConsoleCommand : private FAutoConsoleCommand
	{
		FFUAutoConsoleCommand(const FString& CommandName, const FString& HelpDescription, const FConsoleCommandDelegate& Delegate):
			FAutoConsoleCommand(*CommandName, *HelpDescription, Delegate, ECVF_Default)
		{}
	};
	
	/* No world, with args */
	struct FFUAutoConsoleCommandWithArgs : private FAutoConsoleCommand
	{
		FFUAutoConsoleCommandWithArgs(const FString& CommandName, const FString& HelpDescription, const FConsoleCommandWithArgsDelegate& Delegate):
			FAutoConsoleCommand(*CommandName, *HelpDescription, Delegate, ECVF_Default)
		{}
	};
	
	/* World, no args */
	struct FFUAutoConsoleCommandWithWorld : private FAutoConsoleCommandWithWorld
	{
		FFUAutoConsoleCommandWithWorld(const FString& Category, const FString& CommandName, const FString& HelpDescription, const FConsoleCommandWithWorldDelegate& Delegate):
			FAutoConsoleCommandWithWorld(*CommandName, *HelpDescription, Delegate, ECVF_Default)
		{}
	};
	
	/* World and with args */
	struct FFUAutoConsoleCommandWithWorldAndArgs : private FAutoConsoleCommandWithWorldAndArgs
	{
		FFUAutoConsoleCommandWithWorldAndArgs(const FString& CommandName, const FString& HelpDescription, const FConsoleCommandWithWorldAndArgsDelegate& Delegate):
			FAutoConsoleCommandWithWorldAndArgs(*CommandName, *HelpDescription, Delegate, ECVF_Default)
		{}
	};

	
/**
 * Declares a boolean var and a float var.
 * Use the command as 'My.Console.Command <opt float>', this will toggle the boolean and optionally change the float value
 */
#define FU_CMD_BOOL_WITH_OPT_FLOAT_CPPONLY(Id, Cmd, CmdHelp, BoolVar, FloatVar, DefaultFloatVarValue) \
		static bool BoolVar = false; \
		static float FloatVar = DefaultFloatVarValue; \
		FU_Console::FFUAutoConsoleCommandWithArgs C##Id(Cmd, CmdHelp, \
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
		FU_Console::FFUAutoConsoleCommandWithArgs C##Id(Cmd, CmdHelp, \
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
    FU_Console::FFUAutoConsoleCommand C##Id(Cmd, CmdHelp, \
        FConsoleCommandDelegate::CreateLambda([]() \
        { \
            BoolVar = !BoolVar; \
        }) \
    ); \
    
/** Same as FU_CMD_BOOL_TOGGLE but BoolVar is expected to be declared as extern in the header */
#define FU_CMD_BOOL_TOGGLE_EXTERN(Id, Cmd, CmdHelp, BoolVar, DefaultBoolVal) \
    bool BoolVar = DefaultBoolVal; \
    FU_Console::FFUAutoConsoleCommand C##Id(Cmd, CmdHelp, \
        FConsoleCommandDelegate::CreateLambda([]() \
        { \
            BoolVar = !BoolVar; \
        }) \
    ); \
    
/** Uses FAutoConsoleVariableRef */
#define FU_CMD_AUTOVAR(Id, Cmd, CmdHelp, VarType, VarName, DefaultVarValue) \
	VarType VarName = DefaultVarValue; \
	FAutoConsoleVariableRef Id( \
		TEXT(Cmd), \
		VarName, \
		TEXT(CmdHelp) \
	); \
	
/** Same as FU_CMD_AUTOVAR but VarName is expected to be declared as extern in the header */
#define FU_CMD_AUTOVAR_EXTERN(Id, Cmd, CmdHelp, VarType, VarName, DefaultVarValue) \
	VarType VarName = DefaultVarValue; \
	FAutoConsoleVariableRef Id( \
		TEXT(Cmd), \
		VarName, \
		TEXT(CmdHelp) \
	); \
	
/** Get first instance of given actor class in editor/game world and run given function with optional args */
#define FU_CMD_RUNFUNC_ACTOR_SINGLE(Id, Cmd, CmdHelp, ActorClass, FuncName, ...) \
    FU_Console::FFUAutoConsoleCommand C##Id(Cmd, CmdHelp, \
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
#define FU_CMD_RUNFUNC_ACTOR_ALL(Id, Cmd, CmdHelp, ActorClass, FuncName, ...) \
    FU_Console::FFUAutoConsoleCommand C##Id(Cmd, CmdHelp, \
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

/** Get first instance of given UObject class in editor/game world (excludes CDOs and Archetypes) and run given function with optional args */
#define FU_CMD_RUNFUNC_OBJECT_SINGLE(Id, Cmd, CmdHelp, ObjectClass, FuncName, ...) \
    FU_Console::FFUAutoConsoleCommand C##Id(Cmd, CmdHelp, \
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

/** Get first instance of given UObject class in editor/game world (excludes CDOs and Archetypes) and run given function with optional args */
#define FU_CMD_RUNFUNC_OBJECT_ALL(Id, Cmd, CmdHelp, ObjectClass, FuncName, ...) \
    FU_Console::FFUAutoConsoleCommand C##Id(Cmd, CmdHelp, \
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

#define FU_CMD_RUNLAMBDA(Id, Cmd, CmdHelp, LambdaBody) \
    FU_Console::FFUAutoConsoleCommand C##Id(Cmd, CmdHelp, \
        FConsoleCommandDelegate::CreateLambda([]() \
        { \
			LambdaBody \
        }) \
    ); \

#define FU_CMD_RUNLAMBDA_WORLD_FLOAT0(Id, Cmd, CmdHelp, LambdaBody) \
	FU_Console::FFUAutoConsoleCommandWithWorldAndArgs C##Id(Cmd, CmdHelp, \
		FConsoleCommandWithWorldAndArgsDelegate::CreateLambda([] (const TArray<FString>& Args, UWorld* World) \
		{ \
			if (Args.IsEmpty()) { return; } \
			float Float0 = FCString::Atof(*Args[0]); \
			LambdaBody \
		}) \
	); \
	
#endif
}
