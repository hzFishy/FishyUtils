// By hzFishy - 2025 - Do whatever you want with it.

#include "AI/StateTree/FUTask_CharacterMovementSpeed.h"
#include "StateTreeExecutionContext.h"
#include "GameFramework/CharacterMovementComponent.h"

	
	/*----------------------------------------------------------------------------
		Defaults
	----------------------------------------------------------------------------*/
FFUTask_CharacterMovementSpeed::FFUTask_CharacterMovementSpeed():
	bApplyOnTick(false),
	bApplyOldValueOnExit(true),
	bKeepIfChildrenActive(true)
{
	bShouldCallTick = bApplyOnTick;
}

EStateTreeRunStatus FFUTask_CharacterMovementSpeed::EnterState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	auto& InstanceData = Context.GetInstanceData(*this);
	
	if (!InstanceData.CharacterMovementComponent.IsValid())
	{
		return EStateTreeRunStatus::Failed;
	}

	if (Transition.ChangeType == EStateTreeStateChangeType::Sustained)
	{
		return EStateTreeRunStatus::Running;
	}

	if (MaxWalkSpeedOverride.IsSet())
	{
		InstanceData.OldMaxWalkSpeed = InstanceData.CharacterMovementComponent->MaxWalkSpeed;
		ApplySpeedOverrides(InstanceData);
	}
	
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FFUTask_CharacterMovementSpeed::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	auto& InstanceData = Context.GetInstanceData(*this);

	if (!InstanceData.CharacterMovementComponent.IsValid())
	{
		return EStateTreeRunStatus::Failed;
	}
	
	if (bApplyOnTick)
	{
		ApplySpeedOverrides(InstanceData);
	}
	
	return FStateTreeTaskCommonBase::Tick(Context, DeltaTime);
}

void FFUTask_CharacterMovementSpeed::ExitState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	auto& InstanceData = Context.GetInstanceData(*this);
	
	if (!InstanceData.CharacterMovementComponent.IsValid())
	{
		return;
	}

	// if we move to an active child state we don't want to apply the exit
	if (bKeepIfChildrenActive && Transition.ChangeType == EStateTreeStateChangeType::Sustained)
	{
		return;
	}

	if (bApplyOldValueOnExit && InstanceData.OldMaxWalkSpeed.IsSet())
	{
		InstanceData.CharacterMovementComponent->MaxWalkSpeed = InstanceData.OldMaxWalkSpeed.GetValue();
	}
}

void FFUTask_CharacterMovementSpeed::ApplySpeedOverrides(FFUCharacterMovementSpeedInstanceData& InstanceData) const
{
	if (MaxWalkSpeedOverride.IsSet())
	{
		InstanceData.CharacterMovementComponent->MaxWalkSpeed = MaxWalkSpeedOverride.GetValue();
	}
}
