// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "MeleeCombat/MeleeCombatGameMode.h"
#include "MeleeCombat/MeleeCombat.h"
#include "MeleeCombat/Characters/MeleeCombatCharacter.h"
#include <UObject/ConstructorHelpers.h>

AMeleeCombatGameMode::AMeleeCombatGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/MeleeCombatGame/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
