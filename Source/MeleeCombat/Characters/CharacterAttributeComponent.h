// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <Components/ActorComponent.h>
#include "CharacterAttributeComponent.generated.h"


UCLASS(Blueprintable, Abstract, ClassGroup=(Custom) )
class MELEECOMBAT_API UCharacterAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterAttributeComponent();




	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Attrtibute")
		float BaseValue;
		

	UPROPERTY()
		TArray<TWeakObjectPtr<class UCharacterEffectComponent>> AppliedEffects;

		/** Aborts the move the controller is currently performing */
	 UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Character Attrtibute")
		float GetBaseModifier();


	 UFUNCTION(BlueprintCallable, Category = "Character Attrtibute")
		 void ApplyEffect(class UCharacterEffectComponent* EffectToApply);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
	
};
