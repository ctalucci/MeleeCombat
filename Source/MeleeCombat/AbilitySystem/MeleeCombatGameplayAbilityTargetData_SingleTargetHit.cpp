// Copyright Epic Games, Inc. All Rights Reserved.

#include "MeleeCombatGameplayAbilityTargetData_SingleTargetHit.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MeleeCombatGameplayAbilityTargetData_SingleTargetHit)

struct FGameplayEffectContextHandle;

//////////////////////////////////////////////////////////////////////

void FMeleeCombatGameplayAbilityTargetData_SingleTargetHit::AddTargetDataToContext(FGameplayEffectContextHandle& Context, bool bIncludeActorArray) const
{
	FGameplayAbilityTargetData_SingleTargetHit::AddTargetDataToContext(Context, bIncludeActorArray);

	// Add game-specific data
	//if (FLyraGameplayEffectContext* TypedContext = FLyraGameplayEffectContext::ExtractEffectContext(Context))
	//{
	//	TypedContext->CartridgeID = CartridgeID;
	//}
}

bool FMeleeCombatGameplayAbilityTargetData_SingleTargetHit::NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
{
	FGameplayAbilityTargetData_SingleTargetHit::NetSerialize(Ar, Map, bOutSuccess);

	Ar << StrikeVector;

	return true;
}

