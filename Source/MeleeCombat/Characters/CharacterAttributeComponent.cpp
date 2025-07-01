// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterAttributeComponent.h"
#include "MeleeCombat/MeleeCombat.h"
#include "CharacterEffectComponent.h"


// Sets default values for this component's properties
UCharacterAttributeComponent::UCharacterAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCharacterAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCharacterAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float UCharacterAttributeComponent::GetBaseModifier() 
{
	float TotalBaseModifier = 1.0f;
	for (int32 Idx = 0; Idx < AppliedEffects.Num(); ++Idx)
	{
		
		if (AppliedEffects[Idx].IsValid())
		{
			TotalBaseModifier *= AppliedEffects[Idx]->Modifier;
		}
	}
	return TotalBaseModifier;
}

void UCharacterAttributeComponent::ApplyEffect(class UCharacterEffectComponent* EffectToApply)
{
	if (!EffectToApply) return;
	
	AppliedEffects.Add(EffectToApply);

}
