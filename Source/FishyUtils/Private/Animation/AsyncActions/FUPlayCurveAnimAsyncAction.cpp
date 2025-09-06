// Copyright PushThemAll Dev Team. All Rights Reserved.


#include "Animation/AsyncActions/FUPlayCurveAnimAsyncAction.h"
#include "Animation/FUCurveAnimSubsystem.h"
#include "Asserts/FUAsserts.h"


UFUPlayCurveAnimAsyncAction* UFUPlayCurveAnimAsyncAction::PlayCurveAnim(UObject* WorldContext, UCurveFloat* InCurveFloat, float InPlayRate)
{
	UFUPlayCurveAnimAsyncAction* Node = NewObject<UFUPlayCurveAnimAsyncAction>();
	Node->WorldContext = WorldContext;
	Node->CurveFloat = InCurveFloat;
	Node->PlayRate = InPlayRate;
	
	return Node;
}

void UFUPlayCurveAnimAsyncAction::Activate()
{
	Super::Activate();

	if (IsValid(WorldContext.Get()) && IsValid(CurveFloat.Get()))
	{
		CurveAnimSubsystem = WorldContext->GetWorld()->GetSubsystem<UFUCurveAnimSubsystem>();
		if (FU_ENSURE_WEAKVALID_MSG(CurveAnimSubsystem, "CurveAnimSubsystem not found"))
		{
			CurveAnimSubsystem->PlayCurveAnim(CurveFloat.Get(), PlayRate, [this] (float CurrentTime, float CurrentValue, bool bIsFinished)
			{
				OnPlayCurveAnimTickDelegate.Broadcast(CurrentTime, CurrentValue, bIsFinished);

				if (bIsFinished)
				{
					SetReadyToDestroy();
				}
			});
		}
	}
}
