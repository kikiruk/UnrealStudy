// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BasicUnrealProject : ModuleRules
{
	public BasicUnrealProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        //기본 프로젝트에서 추가된것 : "UMG"
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "NavigationSystem", "AIModule", "GameplayTasks" });

        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        PublicIncludePaths.AddRange(new string[] { "BasicUnrealProject/Public" });
        PrivateIncludePaths.AddRange(new string[] { "BasicUnrealProject/Private" });
        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
