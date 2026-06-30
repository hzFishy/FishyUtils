// By hzFishy - 2026 - Do whatever you want with it.


#include "Draw/FUColors.h"


FColor FU::Colors::Random()
{
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
	
	return Colors[FMath::RandRange(0, Colors.Num() - 1)];
}
