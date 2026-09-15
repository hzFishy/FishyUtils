// By hzFishy - 2026 - Do whatever you want with it.


#include "Draw/FUColors.h"


FColor FU::Colors::PickRandomColor()
{
	static int32 LastIndex = -1;
	
	const int32 RandIndex = FMath::RandRange(0, StaticColors.Num() - 1);
	
	if (RandIndex != LastIndex)
	{
		LastIndex = RandIndex;
		return StaticColors[LastIndex];
	}
	else
	{
		// reroll
		return PickRandomColor();
	}
}

FU::Colors::FFUUniqueColorManager::FFUUniqueColorManager()
{}

void FU::Colors::FFUUniqueColorManager::Init(const TArray<FColor>& InitColors)
{
	FreeColors = InitColors;
	UsedColors.Reserve(UsedColors.Num());
}

FColor FU::Colors::FFUUniqueColorManager::PickRandomColor()
{
	if (FreeColors.IsEmpty())
	{
		// reset
		FreeColors = UsedColors;
		UsedColors.Empty();
	}
	
	const int32 RandIndex = FMath::RandRange(0, FreeColors.Num() - 1);
	const FColor PickedColor = FreeColors[RandIndex];
	FreeColors.RemoveAt(RandIndex);
	UsedColors.Add(PickedColor);
	return PickedColor;
}

void FU::Colors::MakeUniqueColorManager(FFUUniqueColorManager& Ref, bool bReset)
{
	if (bReset)
	{
		Ref = FFUUniqueColorManager();
	}
	Ref.Init(StaticColors);
}

void FU::Colors::MakeUniqueColorManagerWithColors(FFUUniqueColorManager& Ref, const TArray<FColor>& Colors)
{
	Ref = FFUUniqueColorManager();
	Ref.Init(Colors);
}
