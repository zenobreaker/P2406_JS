// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class P2406_JS : ModuleRules
{
	public P2406_JS(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.Add(ModuleDirectory);

        PublicDependencyModuleNames.AddRange(new string[] 
		{    
			"Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "GameplayCameras",
            "GameplayTasks",
            "NavigationSystem", 
            "CableComponent",
            "Niagara",
            "AIModule"
        });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
