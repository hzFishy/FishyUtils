// By hzFishy - 2026 - Do whatever you want with it.

using UnrealBuildTool;

public class FishyUtils : ModuleRules
{
	public FishyUtils(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core", "CoreUObject",
				"BlueprintComponentReference",
				"DeveloperSettings", 
				"UMG", 
				"StateTreeModule"
			}
		);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Engine",
				"Slate", "SlateCore",
				"InputCore",
				"GameplayTags", 
				"LevelSequence", "MovieScene"
			}
		);
		
		if (Target.bBuildEditor)
		{
			bUseUnity = false;
			
			PrivateDependencyModuleNames.AddRange(new string[]
			{
				"UnrealEd",
			});
		}
	}
}
