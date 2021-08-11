// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BethesdaInternship : ModuleRules
{
	public BethesdaInternship(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule" });
    }
}
