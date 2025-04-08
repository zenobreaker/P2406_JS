using UnrealBuildTool;

public class SkillEditor : ModuleRules
{
	public SkillEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PrivateIncludePaths.Add(ModuleDirectory);

        PublicDependencyModuleNames.Add("Core");

        PrivateDependencyModuleNames.Add("CoreUObject");
        PrivateDependencyModuleNames.Add("Engine");
        PrivateDependencyModuleNames.Add("Slate");
        PrivateDependencyModuleNames.Add("SlateCore");

        PrivateDependencyModuleNames.Add("UnrealEd");
        PrivateDependencyModuleNames.Add("InputCore");

        PrivateDependencyModuleNames.Add("P2406_JS");
    }
}
