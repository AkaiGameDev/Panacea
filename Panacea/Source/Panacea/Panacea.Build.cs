// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Panacea : ModuleRules
{
    public Panacea(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore",
            "EnhancedInput", "UMG","Json", "JsonUtilities"
            , "MetasoundEngine"
        });

        PublicIncludePaths.AddRange(
            new string[]
            {
                "Panacea"
            });

    }
}
