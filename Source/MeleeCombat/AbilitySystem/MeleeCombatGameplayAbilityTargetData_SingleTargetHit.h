// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Abilities/GameplayAbilityTargetTypes.h"

#include "MeleeCombatGameplayAbilityTargetData_SingleTargetHit.generated.h"

class FArchive;
struct FGameplayEffectContextHandle;


/** Game-specific additions to SingleTargetHit tracking */
USTRUCT()
struct FMeleeCombatGameplayAbilityTargetData_SingleTargetHit : public FGameplayAbilityTargetData_SingleTargetHit
{
	GENERATED_BODY()

	FMeleeCombatGameplayAbilityTargetData_SingleTargetHit()
		: StrikeVector(0,0,0)
	{ }

	FMeleeCombatGameplayAbilityTargetData_SingleTargetHit(FVector InStrikeVector)
		: StrikeVector(MoveTemp(InStrikeVector))
	{ }

	virtual void AddTargetDataToContext(FGameplayEffectContextHandle& Context, bool bIncludeActorArray) const override;

	/** ID to allow the identification of multiple bullets that were part of the same cartridge */
	UPROPERTY()
	FVector StrikeVector;


	virtual FVector GetEndPoint() const override
	{
		return StrikeVector;
	}

	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);

	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FMeleeCombatGameplayAbilityTargetData_SingleTargetHit::StaticStruct();
	}

};

template<>
struct TStructOpsTypeTraits<FMeleeCombatGameplayAbilityTargetData_SingleTargetHit> : public TStructOpsTypeTraitsBase2<FMeleeCombatGameplayAbilityTargetData_SingleTargetHit>
{
	enum
	{
		WithNetSerializer = true	// For now this is REQUIRED for FGameplayAbilityTargetDataHandle net serialization to work
	};
};

