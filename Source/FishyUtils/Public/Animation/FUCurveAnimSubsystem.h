// By hzFishy 2025 - Do whatever you want with it
// Special thanks to TomLooman who inspired this system (see https://github.com/tomlooman/ActionRoguelike)

#pragma once

#include "CoreMinimal.h"
#include "Kismet/KismetMathLibrary.h"
#include "Subsystems/WorldSubsystem.h"
#include "FUCurveAnimSubsystem.generated.h"
class UCurveFloat;


USTRUCT()
struct FFUActiveCurveAnim
{
	GENERATED_BODY()

	FFUActiveCurveAnim();

	FFUActiveCurveAnim(UCurveFloat* InCurve, TFunction<void (float, float, bool bFinished)> InCallback, float InRate = 1.0f);

	UPROPERTY()
	TObjectPtr<UCurveFloat> Curve;

	float PlayRate;

	/* Current time along curve */
	float CurrentTime;

	/* Cached max time to know when we finished */
	float MaxTime;

	TFunction<void (float CurrentTime, float CurrentValue, bool bFinished)> Callback;

	void Tick(float DeltaTime);

	bool IsFinished() const;
};


USTRUCT()
struct FFUActiveEasingFunc
{
	GENERATED_BODY()

	FFUActiveEasingFunc();

	FFUActiveEasingFunc(float InEasingExp, float InPlayRate, TFunction<void (float)> InCallback);

	float EasingExp;
	
	float PlayRate;
	
	float Time;

	TFunction<void(float)> Callback;

	void Tick(float DeltaTime);

	bool IsFinished();
};


/**
 * 
 */
UCLASS()
class FISHYUTILS_API UFUCurveAnimSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

	TArray<FFUActiveCurveAnim> ActiveAnims;

	TArray<FFUActiveEasingFunc> ActiveEasingFuncs;

	virtual void Tick(float DeltaTime) override;
	
	virtual TStatId GetStatId() const override;

public:

	/** Start animation based on curve */
	void PlayCurveAnim(UCurveFloat* InCurveAsset, float InPlayRate, const TFunction<void(float CurrentTime, float CurrentValue, bool bFinished)>& Func);

	/** Play easing function with output between 0.0 - 1.0 */
	void PlayEasingFunc(EEasingFunc::Type EasingType, float EasingExp, float InPlayRate, const TFunction<void(float)>& Func);
};
