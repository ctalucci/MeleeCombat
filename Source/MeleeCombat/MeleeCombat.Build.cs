// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;



public class MeleeCombat : ModuleRules
{
	public MeleeCombat(ReadOnlyTargetRules Target) : base(Target)

    {
        PrivatePCHHeaderFile = "MeleeCombat.h";
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "RHI","AnalyticsBlueprintLibrary",
            "GameplayAbilities", "GameplayTags", "GameplayTasks",
            "RenderCore",
            "OnlineSubsystem",
            "OnlineSubsystemUtils"
             });
	}
}
