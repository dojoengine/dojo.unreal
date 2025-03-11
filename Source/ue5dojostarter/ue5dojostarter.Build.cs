// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ue5dojostarter : ModuleRules
{
	public ue5dojostarter(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "Dojo" });
	}
}
