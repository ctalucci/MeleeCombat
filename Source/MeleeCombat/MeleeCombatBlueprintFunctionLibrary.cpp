// Fill out your copyright notice in the Description page of Project Settings.

#include "MeleeCombatBlueprintFunctionLibrary.h"
#include "AbilitySystem/MeleeCombatGameplayAbilityTargetData_SingleTargetHit.h"



FGameplayAbilityTargetDataHandle UMeleeCombatBlueprintFunctionLibrary::AbilityTargetDataFromHitResultAndStrikeVector(const FHitResult& HitResult, const FVector& StrikeVector)
{
	FMeleeCombatGameplayAbilityTargetData_SingleTargetHit* NewTargetData = new FMeleeCombatGameplayAbilityTargetData_SingleTargetHit();
	NewTargetData->HitResult = HitResult;
	NewTargetData->StrikeVector = StrikeVector;

	// Give it a handle and return
	FGameplayAbilityTargetDataHandle Handle;
	Handle.Add(NewTargetData);

	return Handle;
}


FVector UMeleeCombatBlueprintFunctionLibrary::GetStrikeVectorFromTargetData(const FGameplayAbilityTargetDataHandle& TargetDataHandle, int32 Index)
{
	if (TargetDataHandle.Data.IsValidIndex(Index))
	{
		if (auto data = TargetDataHandle.Data[0].Get())
		{
			return data->GetEndPoint();
		}
	}

	return FVector();
}