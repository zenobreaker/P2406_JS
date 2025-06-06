// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class EditorCommon : ModuleRules
{
	public EditorCommon(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

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
