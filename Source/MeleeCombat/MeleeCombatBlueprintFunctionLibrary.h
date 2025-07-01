// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Kismet/BlueprintFunctionLibrary.h>
#include "Abilities/GameplayAbilityTargetTypes.h"

#include "MeleeCombatBlueprintFunctionLibrary.generated.h"


/**
 * 
 */
UCLASS()
class UMeleeCombatBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/** Creates a target data with a single hit result */
	UFUNCTION(BlueprintPure, Category = "Ability|TargetData")
	static FGameplayAbilityTargetDataHandle	AbilityTargetDataFromHitResultAndStrikeVector(const FHitResult& HitResult, const FVector& StrikeVector);

		/** Returns the hit result for a given index if it exists */
	UFUNCTION(BlueprintPure, Category = "Ability|TargetData")
	static FVector GetStrikeVectorFromTargetData(const FGameplayAbilityTargetDataHandle& HitResultAndStrikeVector, int32 Index);

};
