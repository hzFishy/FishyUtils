// By hzFishy - 2026 - Do whatever you want with it.


#include "Animation/FUCurveAnimSubsystem.h"


FFUActiveCurveAnim::FFUActiveCurveAnim():
	PlayRate(1),
	CurrentTime(0),
	MaxTime(0)
{}

FFUActiveCurveAnim::FFUActiveCurveAnim(UCurveFloat* InCurve, TFunction<void(float, float, bool bFinished)> InCallback, float InRate):
	Curve(InCurve),
	PlayRate(InRate),
	CurrentTime(0),
	MaxTime(0),
	Callback(InCallback)
{
	// We use first keyframe as start time, you could choose to always start at 0.0f
	Curve->GetTimeRange(CurrentTime, MaxTime);
}

void FFUActiveCurveAnim::Tick(float DeltaTime)
{
	CurrentTime += DeltaTime * PlayRate;

	float CurrentValue = Curve->GetFloatValue(CurrentTime);

	Callback(CurrentTime, CurrentValue, IsFinished());
}

bool FFUActiveCurveAnim::IsFinished() const
{
	return CurrentTime >= MaxTime;
}


FFUActiveEasingFunc::FFUActiveEasingFunc():
	EasingExp(2),
	PlayRate(1),
	Time(0)
{}

FFUActiveEasingFunc::FFUActiveEasingFunc(float InEasingExp, float InPlayRate, TFunction<void(float)> InCallback):
	EasingExp(InEasingExp),
	PlayRate(InPlayRate),
	Time(0)
{}

void FFUActiveEasingFunc::Tick(float DeltaTime)
{
	Time += (DeltaTime*PlayRate);

	// Clamp
	Time = FMath::Min(Time, 1.0f);

	float CurrentValue = FMath::InterpEaseInOut(0.0f, 1.0f, Time, EasingExp);

	Callback(CurrentValue);
}

bool FFUActiveEasingFunc::IsFinished()
{
	return Time >= 1.0f;
}


void UFUCurveAnimSubsystem::Tick(float DeltaTime)
{
	// Curve Based Anims
	for (auto It = ActiveAnims.CreateIterator(); It; ++It)
	{
		It->Tick(DeltaTime);

		if (It->IsFinished())
		{
			It.RemoveCurrent();
		}
	}

	// Easing Functions
	for (auto It = ActiveEasingFuncs.CreateIterator(); It; ++It)
	{
		It->Tick(DeltaTime);

		if (It->IsFinished())
		{
			It.RemoveCurrent();
		}
	}
}

TStatId UFUCurveAnimSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(FUCurveAnimSubsystem, STATGROUP_Tickables);
}

void UFUCurveAnimSubsystem::PlayCurveAnim(UCurveFloat* InCurveAsset, float InPlayRate, const TFunction<void (float CurrentTime, float CurrentValue, bool bFinished)>& Func)
{
	check(InCurveAsset);

	ActiveAnims.Add(FFUActiveCurveAnim(InCurveAsset, Func, InPlayRate));
}

void UFUCurveAnimSubsystem::PlayEasingFunc(EEasingFunc::Type EasingType, float EasingExp, float InPlayRate, const TFunction<void(float)>& Func)
{
	check(EasingType == EEasingFunc::EaseInOut);

	ActiveEasingFuncs.Add(FFUActiveEasingFunc(EasingExp, InPlayRate, Func));
}
