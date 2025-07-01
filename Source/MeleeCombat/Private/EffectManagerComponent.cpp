// Fill out your copyright notice in the Description page of Project Settings.

#include "EffectManagerComponent.h"
#include "MeleeCombat/MeleeCombat.h"
#include "CharacterEffectComponent.h"
#include "CharacterAttributeComponent.h"


// Sets default values for this component's properties
UEffectManagerComponent::UEffectManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UEffectManagerComponent::ApplyStatusEffect(AStatusEffect* EffectToApply)
{
	if (!EffectToApply || !GetOwner()) return;

	TArray<UActorComponent*> Components;
	EffectToApply->GetComponents(UCharacterEffectComponent::StaticClass(), Components);
	for (auto Comp : Components)
	{
		if (UCharacterEffectComponent* CharEffect = Cast<UCharacterEffectComponent>(Comp))
		{	
			if (UCharacterAttributeComponent* AttributeToEffect = Cast<UCharacterAttributeComponent>(GetOwner()->GetComponentByClass(CharEffect->CharacterAttributeToEffect)))
			{
				AttributeToEffect->ApplyEffect(CharEffect);
			}
			
		}
		
	}

	OnAppliedEffect.Broadcast(EffectToApply);
	CurrentEffects.Add(EffectToApply);

}

// Called when the game starts
void UEffectManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UEffectManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

