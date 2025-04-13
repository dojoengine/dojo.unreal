// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class Dojo : ModuleRules
{
    protected readonly string Version = "1.3.1";
    protected string VersionPath { get => Path.Combine(ModuleDirectory, Version); }
    protected string LibraryPath { get => Path.Combine(VersionPath, "lib"); }

    public Dojo(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        
        PublicSystemIncludePaths.Add(Path.Combine(VersionPath, "include"));

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
            }
        );

        if (Target.Platform == UnrealTargetPlatform.Mac)
        {
            string DylibPath = Path.Combine(LibraryPath, "Mac", "libdojo_c.dylib");
            PublicAdditionalLibraries.Add(DylibPath);
            
            // Add runtime dependency for Mac
            RuntimeDependencies.Add("$(PluginDir)/Source/Dojo/" + Version + "/lib/Mac/libdojo_c.dylib");
            
            // Delay load the dylib
            PublicDelayLoadDLLs.Add(Path.Combine(LibraryPath, "Mac", "libdojo_c.dylib"));
        }
        else if (Target.Platform == UnrealTargetPlatform.IOS)
        {
            // Use iOS library
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, Version, "lib", "IOS", "libdojo_c.a"));
        }
    }
}
