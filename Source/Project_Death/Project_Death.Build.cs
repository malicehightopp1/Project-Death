// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Project_Death : ModuleRules
{
	public Project_Death(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate",
			"SlateCore",
			"Niagara"
		});

		if (Target.Type == TargetType.Editor)
		{
			PrivateDependencyModuleNames.Add("UMGEditor");
		}

		PublicIncludePaths.AddRange(new string[] {
			"Project_Death",
			"Project_Death/Variant_Platforming",
			"Project_Death/Variant_Platforming/Animation",
			"Project_Death/Variant_Combat",
			"Project_Death/Variant_Combat/AI",
			"Project_Death/Variant_Combat/Animation",
			"Project_Death/Variant_Combat/Gameplay",
			"Project_Death/Variant_Combat/Interfaces",
			"Project_Death/Variant_Combat/UI",
			"Project_Death/Variant_SideScrolling",
			"Project_Death/Variant_SideScrolling/AI",
			"Project_Death/Variant_SideScrolling/Gameplay",
			"Project_Death/Variant_SideScrolling/Interfaces",
			"Project_Death/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
