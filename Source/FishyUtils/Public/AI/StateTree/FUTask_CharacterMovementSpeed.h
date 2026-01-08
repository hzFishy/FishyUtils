// By hzFishy - 2025 - Do whatever you want with it.

#pragma once

#include "StateTreeTaskBase.h"
#include "FUTask_CharacterMovementSpeed.generated.h"
class UCharacterMovementComponent;


USTRUCT()
struct FFUCharacterMovementSpeedInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Input")
	TWeakObjectPtr<UCharacterMovementComponent> CharacterMovementComponent;

	TOptional<float> OldMaxWalkSpeed;
};

/**
 * While this task is active we change the CMC max walk speed.
 * On exit we set it back to the previously cached value.
 */
USTRUCT(DisplayName="Task Character Movement Speed", Category="Fishy Utils")
struct FISHYUTILS_API FFUTask_CharacterMovementSpeed : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FFUCharacterMovementSpeedInstanceData;

	
	/*----------------------------------------------------------------------------
		Text
	----------------------------------------------------------------------------*/
protected:
	UPROPERTY(EditAnywhere, Category="Parameter")
	TOptional<float> MaxWalkSpeedOverride;

	UPROPERTY(EditAnywhere, Category="Parameter")
	bool bApplyOnTick;
	
	UPROPERTY(EditAnywhere, Category="Parameter")
	bool bApplyOldValueOnExit;
	
	UPROPERTY(EditAnywhere, Category="Parameter")
	bool bKeepIfChildrenActive;
	
	
	/*----------------------------------------------------------------------------
		Defaults
	----------------------------------------------------------------------------*/
public:
	FFUTask_CharacterMovementSpeed();
	
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
	
	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

	
	/*----------------------------------------------------------------------------
		Core
	----------------------------------------------------------------------------*/
protected:
	void ApplySpeedOverrides(FFUCharacterMovementSpeedInstanceData& InstanceData) const;
};
