// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Puck : ModuleRules
{
	public Puck(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" ,  "Niagara" , "UMG", "GameplayCameras"});
	}
}
