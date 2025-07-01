// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class MeleeCombatEditorTarget : TargetRules
{
	public MeleeCombatEditorTarget(TargetInfo Target) : base(Target)
	{
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        DefaultBuildSettings = BuildSettingsVersion.V5;

        Type = TargetType.Editor;
		ExtraModuleNames.Add("MeleeCombat");
	}

	//
	// TargetRules interface.
	//
}
