// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Kismet/BlueprintFunctionLibrary.h>
#include "AnimBlueprintFunctionLibrary.generated.h"



/**  */
UENUM()
namespace ETraceSectionType
{
	enum Type
	{
		/** */
		BeforeTrace,
		/** */
		DuringTrace,
		/** */
		AfterTrace,
	};
}


/**
 * 
 */
UCLASS()
class UAnimBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, BlueprintPure, Category=Animation)
	static	float GetDirectionYaw(APawn* Pawn);

	UFUNCTION(BlueprintCallable, Category = Animation)
	static void PlayMontageSectionWithDuration(USkeletalMeshComponent* MeshComp, UAnimMontage* Montage, float Duration, FName SectionName);

	UFUNCTION(BlueprintCallable, Category = Animation)
	static void PlayMontageTracingSectionWithDuration(USkeletalMeshComponent* MeshComp, UAnimMontage* Montage, float Duration, ETraceSectionType::Type Section);
};
