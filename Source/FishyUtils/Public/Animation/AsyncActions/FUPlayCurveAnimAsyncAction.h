// Copyright PushThemAll Dev Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "FUPlayCurveAnimAsyncAction.generated.h"
class UFUCurveAnimSubsystem;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FFUOnPlayCurveAnimTickOutputPin, float, CurrentTime, float, CurrentValue, bool, bFinished);


/**
 * 
 */
UCLASS()
class FISHYUTILS_API UFUPlayCurveAnimAsyncAction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, DisplayName="Play Curve Anim", meta = (WorldContext = "WorldContext", BlueprintInternalUseOnly = "true"), Category="FishyUtils|Animation")
	static UFUPlayCurveAnimAsyncAction* PlayCurveAnim(UObject* WorldContext, UCurveFloat* InCurveFloat, float InPlayRate = 1);

	UPROPERTY(BlueprintAssignable, DisplayName = "On Play Curve Anim Tick")
	FFUOnPlayCurveAnimTickOutputPin OnPlayCurveAnimTickDelegate;
	
protected:
	virtual void Activate() override;

	UPROPERTY()
	TObjectPtr<UObject> WorldContext;
	UPROPERTY()
	TObjectPtr<UCurveFloat> CurveFloat;
	float PlayRate;

	TWeakObjectPtr<UFUCurveAnimSubsystem> CurveAnimSubsystem;
};
