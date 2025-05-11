using UnrealBuildTool;

public class FishyUtilsEditor : ModuleRules
{
    public FishyUtilsEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core", "SceneOutliner", "BlueprintComponentReference",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate", "SlateCore", "UMG", "InputCore",
                "FishyUtils", "PropertyEditor",
                "SceneOutliner",
                "ToolMenus", "UnrealEd"
            }
        );
    }
}