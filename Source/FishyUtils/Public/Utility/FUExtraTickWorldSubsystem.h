// By hzFishy - 2026 - Do whatever you want with it.
// Thanks to https://github.com/HomerJohnston/TickerBee for the initial code

#pragma once

#include "Subsystems/WorldSubsystem.h"
#include "FUExtraTickWorldSubsystem.generated.h"


DECLARE_DYNAMIC_DELEGATE_OneParam(FFUExtraTickDynamicDelegate, float, DeltaTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFUExtraTickDynamicMultiDelegate, float, DeltaTime);

using FFUExtraTickDelegate = TDelegate<void(float)>;

UENUM(BlueprintType, DisplayName="Extra Tick Group")
enum class EFUExtraTickGroup : uint8
{
	/** Any item that needs to be executed before physics simulation starts. */
	PrePhysics UMETA(DisplayName="Pre Physics"),
	
	/** Special tick group that starts physics simulation. */							
	StartPhysics UMETA(DisplayName="Start Physics"),
	
	/** Any item that can be run in parallel with our physics simulation work. */
	DuringPhysics UMETA(DisplayName="During Physics"),
	
	/** Special tick group that ends physics simulation. */
	EndPhysics UMETA(DisplayName="End Physics"),
	
	/** Any item that needs rigid body and cloth simulation to be complete before being executed. */
	PostPhysics UMETA(DisplayName="Post Physics"),
	
	/** Any item that needs the update work to be done before being ticked. */
	PostUpdateWork UMETA(DisplayName="Post Update Work"),
	
	/** Catchall for anything demoted to the end. */
	LastDemotable UMETA(DisplayName = "Last Demotable"),
};


USTRUCT()
struct FFUExtraTickFunction : public FTickFunction
{
	GENERATED_BODY()
	
	
	/*----------------------------------------------------------------------------
		Properties
	----------------------------------------------------------------------------*/
	UPROPERTY(Transient)
	UFUExtraTickWorldSubsystem* Target;
	
	ETickingGroup Group;
	
	UPROPERTY(Transient)
	FFUExtraTickDynamicMultiDelegate DynamicDelegate;
	
	TMulticastDelegate<void(float)> NativeDelegate;
    
	
	/*----------------------------------------------------------------------------
		Defaults
	----------------------------------------------------------------------------*/
public:
	FFUExtraTickFunction();

	virtual void ExecuteTick(float DeltaTime, ELevelTick TickType, ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionEventGraph) override;

	virtual FString DiagnosticMessage() override;
};

template<>
struct TStructOpsTypeTraits<FFUExtraTickFunction> : public TStructOpsTypeTraitsBase2<FFUExtraTickFunction>
{
	enum
	{
		WithCopy = false
	};
};


UCLASS(DisplayName="Extra Tick World Subsystem")
class FISHYUTILS_API UFUExtraTickWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
	friend FFUExtraTickFunction;
	
	
	/*----------------------------------------------------------------------------
		Properties
	----------------------------------------------------------------------------*/
protected:
	FFUExtraTickFunction PrePhysics;
	
	FFUExtraTickFunction StartPhysics;
	
	FFUExtraTickFunction DuringPhysics;
	
	FFUExtraTickFunction EndPhysics;
	
	FFUExtraTickFunction PostPhysics;
	
	FFUExtraTickFunction PostUpdateWork;
	
	FFUExtraTickFunction LastDemotable;
	
	
	/*----------------------------------------------------------------------------
		Defaults
	----------------------------------------------------------------------------*/
public:
	UFUExtraTickWorldSubsystem();
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	virtual void Deinitialize() override;
	
	
	/*----------------------------------------------------------------------------
		Core
	----------------------------------------------------------------------------*/
public:
	UFUNCTION(BlueprintCallable, DisplayName="Register Callback", Category="FishyUtils|ExtraTick")
	void K2_RegisterCallback(FFUExtraTickDynamicDelegate NewRegistrant, EFUExtraTickGroup TickGroup);

	/** 
	 *  Warning: This will crash if the subsystem wasn't initialized.
	 */
	void RegisterCallback(FFUExtraTickDelegate NewRegistrant, EFUExtraTickGroup TickGroup);

protected:
	void HandleTick(float DeltaTime, ETickingGroup Group);
	
	ETickingGroup GetEngineTickGroup(EFUExtraTickGroup Group);
	
	FFUExtraTickFunction& GetTicker(ETickingGroup TickGroup);
};
