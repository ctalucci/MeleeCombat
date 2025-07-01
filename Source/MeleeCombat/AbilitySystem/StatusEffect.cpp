// Fill out your copyright notice in the Description page of Project Settings.

#include "StatusEffect.h"
#include "MeleeCombat/MeleeCombat.h"
#include <Net/UnrealNetwork.h>
#include "EffectManagerComponent.h"


// Sets default values
AStatusEffect::AStatusEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

// Called when the game starts or when spawned
void AStatusEffect::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner())
	{
		if (auto EffectManager = Cast<UEffectManagerComponent>(GetOwner()->GetComponentByClass(UEffectManagerComponent::StaticClass())))
		{
			EffectManager->ApplyStatusEffect(this);
			SetLifeSpan(EffectDuration);
		}
	}
	
}

// Called every frame
void AStatusEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AStatusEffect::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AStatusEffect, EffectDuration);
}

