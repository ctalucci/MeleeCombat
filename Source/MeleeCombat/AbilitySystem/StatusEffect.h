// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Actor.h>
#include "StatusEffect.generated.h"

UCLASS(Blueprintable, Abstract, ClassGroup = (Custom))
class MELEECOMBAT_API AStatusEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStatusEffect();

	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadOnly, Category = "AttributeEffect")
		float EffectDuration = 0.0f ;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AttributeEffect")
		class UParticleSystem* EffectParticleSystem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AttributeEffect")
	FName EffectDisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AttributeEffect")
		class UTexture2D* EffectUIIcon;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAppliedEffect, AStatusEffect*, EffectApplied);
	UPROPERTY(BlueprintAssignable)
		FOnAppliedEffect OnAppliedEffect;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
