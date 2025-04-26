// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FishyUtils : ModuleRules
{
	public FishyUtils(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
			}
		);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"GameplayTags",
				"InputCore"
			}
		);
		
		if (Target.bBuildEditor)
		{
			bUseUnity = false;
			
			PrivateDependencyModuleNames.AddRange(new string[]
			{
				"UnrealEd",
				"SceneOutliner",
				"ToolMenus"
			});
		}
	}
}
