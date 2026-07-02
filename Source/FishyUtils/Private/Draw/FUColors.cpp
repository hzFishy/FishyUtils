// By hzFishy - 2026 - Do whatever you want with it.


#include "Draw/FUColors.h"


FColor FU::Colors::PickRandomColor()
{
	static int32 LastIndex = -1;
	
	static TArray<FColor, TInlineAllocator<80>> Colors = {
		Red,
		Tomato,
		LightCoral,
		Orange,
		Coral,
		TomatoOrange,
		Apricot,
		Yellow,
		Lemon,
		Green,
		ForestGreen,
		Olive,
		PaleGreen,
		SeaGreen,
		SpringGreen,
		Blue,
		SteelBlue,
		LightBlue,
		MidnightBlue,
		Cyan,
		Purple,
		Plum ,
		Magenta,
		DarkViolet,
		Indigo,
		Pink,
		DeepPink,
		Fuchsia,
		PastelBlue,
		PastelGreen,
		PastelPurple,
		PastelRed,
		PastelYellow,
		PastelOrange,
		NightSky,
		MidnightFog,
		BloodMoon,
		ToxicGreen,
		PlasmaPurple,
		CyberBlue,
		AshenGray,
		SteampunkBronze,
		ToxicSlime
	};
	
	const int32 RandIndex = FMath::RandRange(0, Colors.Num() - 1);
	
	if (RandIndex != LastIndex)
	{
		LastIndex = RandIndex;
		return Colors[LastIndex];
	}
	else
	{
		// reroll
		return PickRandomColor();
	}
}
