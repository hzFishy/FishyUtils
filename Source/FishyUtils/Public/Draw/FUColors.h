// By hzFishy 2025 - Do whatever you want with it

#pragma once

// Thanks to Baffled for these generated colors!

namespace FU::Colors
{
	// Basics
	inline constexpr FColor Black       			= FColor(0, 0, 0, 255);
	inline constexpr FColor White       			= FColor(255, 255, 255, 255);
	inline constexpr FColor Gray        			= FColor(128, 128, 128, 255);
	inline constexpr FColor DarkGray    			= FColor(64, 64, 64, 255);
	inline constexpr FColor LightGray   			= FColor(192, 192, 192, 255);
	inline constexpr FColor Transparent 			= FColor(0, 0, 0, 0);

	// Reds
	inline constexpr FColor Red             		= FColor(255, 0, 0, 255);
	inline constexpr FColor Crimson         		= FColor(220, 20, 60, 255);
	inline constexpr FColor DarkRed         		= FColor(139, 0, 0, 255);
	inline constexpr FColor Tomato          		= FColor(255, 99, 71, 255);
	inline constexpr FColor Firebrick       		= FColor(178, 34, 34, 255);
	inline constexpr FColor LightCoral      		= FColor(240, 128, 128, 255);
	inline constexpr FColor IndianRed       		= FColor(205, 92, 92, 255);
	inline constexpr FColor Salmon          		= FColor(250, 128, 114, 255);
	inline constexpr FColor DarkSalmon      		= FColor(233, 150, 122, 255);

	// Oranges
	inline constexpr FColor Orange          		= FColor(255, 165, 0, 255);
	inline constexpr FColor DarkOrange      		= FColor(255, 140, 0, 255);
	inline constexpr FColor Coral           		= FColor(255, 127, 80, 255);
	inline constexpr FColor TomatoOrange    		= FColor(255, 69, 0, 255);
	inline constexpr FColor Peach           		= FColor(255, 218, 185, 255);
	inline constexpr FColor Apricot         		= FColor(251, 206, 177, 255);
	inline constexpr FColor Amber           		= FColor(255, 191, 0, 255);

	// Yellows
	inline constexpr FColor Yellow          		= FColor(255, 255, 0, 255);
	inline constexpr FColor Gold            		= FColor(255, 215, 0, 255);
	inline constexpr FColor Khaki           		= FColor(240, 230, 140, 255);
	inline constexpr FColor Lemon           		= FColor(253, 255, 0, 255);
	inline constexpr FColor LightGoldenrod  		= FColor(250, 250, 210, 255);
	inline constexpr FColor Mustard         		= FColor(255, 219, 88, 255);
	inline constexpr FColor PaleGoldenrod   		= FColor(238, 232, 170, 255);

	// Greens
	inline constexpr FColor Green           		= FColor(0, 255, 0, 255);
	inline constexpr FColor ForestGreen     		= FColor(34, 139, 34, 255);
	inline constexpr FColor Lime            		= FColor(50, 205, 50, 255);
	inline constexpr FColor Olive           		= FColor(128, 128, 0, 255);
	inline constexpr FColor DarkOliveGreen  		= FColor(85, 107, 47, 255);
	inline constexpr FColor PaleGreen       		= FColor(152, 251, 152, 255);
	inline constexpr FColor SeaGreen        		= FColor(46, 139, 87, 255);
	inline constexpr FColor SpringGreen     		= FColor(0, 255, 127, 255);
	inline constexpr FColor Mint            		= FColor(189, 252, 201, 255);
	inline constexpr FColor NeonGreen       		= FColor(57, 255, 20, 255);

	// Blues
	inline constexpr FColor Blue            		= FColor(0, 0, 255, 255);
	inline constexpr FColor SkyBlue         		= FColor(135, 206, 235, 255);
	inline constexpr FColor SteelBlue       		= FColor(70, 130, 180, 255);
	inline constexpr FColor LightBlue       		= FColor(173, 216, 230, 255);
	inline constexpr FColor DeepSkyBlue     		= FColor(0, 191, 255, 255);
	inline constexpr FColor Navy            		= FColor(0, 0, 128, 255);
	inline constexpr FColor MidnightBlue    		= FColor(25, 25, 112, 255);
	inline constexpr FColor Cyan            		= FColor(0, 255, 255, 255);
	inline constexpr FColor Aqua            		= FColor(0, 255, 255, 255);
	inline constexpr FColor Turquoise       		= FColor(64, 224, 208, 255);

	// Purples
	inline constexpr FColor Purple          		= FColor(128, 0, 128, 255);
	inline constexpr FColor Lavender        		= FColor(230, 230, 250, 255);
	inline constexpr FColor Plum            		= FColor(221, 160, 221, 255);
	inline constexpr FColor Violet          		= FColor(238, 130, 238, 255);
	inline constexpr FColor Orchid          		= FColor(218, 112, 214, 255);
	inline constexpr FColor Magenta         		= FColor(255, 0, 255, 255);
	inline constexpr FColor DarkViolet      		= FColor(148, 0, 211, 255);
	inline constexpr FColor Indigo          		= FColor(75, 0, 130, 255);
	inline constexpr FColor Thistle         		= FColor(216, 191, 216, 255);

	// Pinks
	inline constexpr FColor Pink            		= FColor(255, 192, 203, 255);
	inline constexpr FColor HotPink         		= FColor(255, 105, 180, 255);
	inline constexpr FColor DeepPink        		= FColor(255, 20, 147, 255);
	inline constexpr FColor LightPink       		= FColor(255, 182, 193, 255);
	inline constexpr FColor PaleVioletRed   		= FColor(219, 112, 147, 255);
	inline constexpr FColor Fuchsia         		= FColor(255, 0, 255, 255);
	inline constexpr FColor Bubblegum       		= FColor(255, 193, 204, 255);

	// Desat
	inline constexpr FColor PastelBlue      		= FColor(174, 198, 207, 255);
	inline constexpr FColor PastelGreen     		= FColor(119, 221, 119, 255);
	inline constexpr FColor PastelPurple    		= FColor(177, 156, 217, 255);
	inline constexpr FColor PastelRed       		= FColor(255, 105, 97, 255);
	inline constexpr FColor PastelYellow    		= FColor(253, 253, 150, 255);
	inline constexpr FColor PastelOrange    		= FColor(255, 179, 71, 255);

	// Stylized
	inline constexpr FColor NightSky         		= FColor(20, 24, 82, 255);
	inline constexpr FColor MidnightFog      		= FColor(50, 50, 90, 255);
	inline constexpr FColor BloodMoon        		= FColor(180, 30, 30, 255);
	inline constexpr FColor ToxicGreen       		= FColor(110, 255, 110, 255);
	inline constexpr FColor PlasmaPurple     		= FColor(200, 80, 255, 255);
	inline constexpr FColor CyberBlue        		= FColor(0, 180, 255, 255);
	inline constexpr FColor GhostWhite       		= FColor(248, 248, 255, 255);
	inline constexpr FColor AshenGray        		= FColor(105, 105, 105, 255);
	inline constexpr FColor SteampunkBronze  		= FColor(205, 127, 50, 255);
	inline constexpr FColor ToxicSlime       		= FColor(173, 255, 47, 255);
}
