// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <Components/ActorComponent.h>
#include "CharacterEffectComponent.generated.h"


UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent), ClassGroup=(Custom) )
class MELEECOMBAT_API UCharacterEffectComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterEffectComponent();


	UPROPERTY(EditDefaultsOnly, Category = "Effect Values")
		float Modifier = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Effect Values")
		TSubclassOf<class UCharacterAttributeComponent> CharacterAttributeToEffect;



protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
	
};
