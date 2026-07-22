// By hzFishy - 2026 - Do whatever you want with it.


#include "Utility/FUExtraTickWorldSubsystem.h"

	
	/*----------------------------------------------------------------------------
		Defaults
	----------------------------------------------------------------------------*/
FFUExtraTickFunction::FFUExtraTickFunction(): 
	Target(nullptr), 
	Group(TG_MAX)
{
	// TODO: make this customizable per registrant
	
	bCanEverTick = true;
	bHighPriority = false;
	bRunOnAnyThread = false;
	bTickEvenWhenPaused = false;
	TickInterval = 0.0f;
}

void FFUExtraTickFunction::ExecuteTick(float DeltaTime, ELevelTick TickType, ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionEventGraph)
{
	if (IsValid(Target) && TickType != LEVELTICK_ViewportsOnly)
	{
		Target->HandleTick(DeltaTime, TickGroup);
	}
}

FString FFUExtraTickFunction::DiagnosticMessage()
{
	return "FFUExtraTickFunction";
}

	
	/*----------------------------------------------------------------------------
		Defaults
	----------------------------------------------------------------------------*/
UFUExtraTickWorldSubsystem::UFUExtraTickWorldSubsystem()
{}

void UFUExtraTickWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	PrePhysics.Target = this;
    PrePhysics.TickGroup = ETickingGroup::TG_PrePhysics;
	PrePhysics.RegisterTickFunction(GetWorld()->PersistentLevel.Get());
	
    StartPhysics.Target = this;
    StartPhysics.TickGroup = ETickingGroup::TG_StartPhysics;
	StartPhysics.RegisterTickFunction(GetWorld()->PersistentLevel.Get());
	
    DuringPhysics.Target = this;
    DuringPhysics.TickGroup = ETickingGroup::TG_DuringPhysics;
	DuringPhysics.RegisterTickFunction(GetWorld()->PersistentLevel.Get());
	
    EndPhysics.Target = this;
    EndPhysics.TickGroup = ETickingGroup::TG_EndPhysics;
	EndPhysics.RegisterTickFunction(GetWorld()->PersistentLevel.Get());
	
    PostPhysics.Target = this;
    PostPhysics.TickGroup = ETickingGroup::TG_PostPhysics;
	PostPhysics.RegisterTickFunction(GetWorld()->PersistentLevel.Get());
	
    PostUpdateWork.Target = this;
    PostUpdateWork.TickGroup = ETickingGroup::TG_PostUpdateWork;
	PostUpdateWork.RegisterTickFunction(GetWorld()->PersistentLevel.Get());
	
    LastDemotable.Target = this;
    LastDemotable.TickGroup = ETickingGroup::TG_LastDemotable;
	LastDemotable.RegisterTickFunction(GetWorld()->PersistentLevel.Get());
}

void UFUExtraTickWorldSubsystem::Deinitialize()
{
	Super::Deinitialize();
	
	PrePhysics.UnRegisterTickFunction();
	
	StartPhysics.UnRegisterTickFunction();
	
	DuringPhysics.UnRegisterTickFunction();
	
	EndPhysics.UnRegisterTickFunction();
	
	PostPhysics.UnRegisterTickFunction();
	
	PostUpdateWork.UnRegisterTickFunction();
	
	LastDemotable.UnRegisterTickFunction();
}

	
	/*----------------------------------------------------------------------------
		Core
	----------------------------------------------------------------------------*/
void UFUExtraTickWorldSubsystem::K2_RegisterCallback(FFUExtraTickDynamicDelegate NewRegistrant, EFUExtraTickGroup TickGroup)
{
	GetTicker(GetEngineTickGroup(TickGroup)).DynamicDelegate.Add(NewRegistrant);
}

void UFUExtraTickWorldSubsystem::RegisterCallback(FFUExtraTickDelegate NewRegistrant, EFUExtraTickGroup TickGroup)
{
	GetTicker(GetEngineTickGroup(TickGroup)).NativeDelegate.Add(NewRegistrant);
}

void UFUExtraTickWorldSubsystem::HandleTick(float DeltaTime, ETickingGroup Group)
{
	FFUExtraTickFunction& Ticker = GetTicker(Group);
	Ticker.NativeDelegate.Broadcast(DeltaTime);
	Ticker.DynamicDelegate.Broadcast(DeltaTime);
}

ETickingGroup UFUExtraTickWorldSubsystem::GetEngineTickGroup(EFUExtraTickGroup Group)
{
	return static_cast<ETickingGroup>(Group);
}

FFUExtraTickFunction& UFUExtraTickWorldSubsystem::GetTicker(ETickingGroup TickGroup)
{
	switch (TickGroup)
	{
	case TG_PrePhysics:
		{
			return PrePhysics;
		}
	case TG_StartPhysics:
		{
			return StartPhysics;
		}
	case TG_DuringPhysics:
		{
			return DuringPhysics;
		}
	case TG_EndPhysics:
		{
			return EndPhysics;
		}
	case TG_PostPhysics:
		{
			return PostPhysics;
		}
	case TG_PostUpdateWork:
		{
			return PostUpdateWork;
		}
	case TG_LastDemotable:
		{
			return LastDemotable;
		}
	default:
		{
			checkNoEntry();
			return PrePhysics;
		}
	}
}
