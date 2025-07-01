// Fill out your copyright notice in the Description page of Project Settings.

#include "ClientAuthorityAbilityActor.h"
#include "MeleeCombat/MeleeCombat.h"
#include "AbilityComponent.h"
#include <Net/UnrealNetwork.h>
#include <GameFramework/Pawn.h>
#include <GameFramework/Controller.h>

// Sets default values
AClientAuthorityAbilityActor::AClientAuthorityAbilityActor(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

// Called when the game starts or when spawned
void AClientAuthorityAbilityActor::BeginPlay()
{
	Super::BeginPlay();


	if (AActor* AbilityOwner = GetOwner())
	{
		UAbilityComponent* AbilityComp = Cast<UAbilityComponent>(AbilityOwner->GetComponentByClass(UAbilityComponent::StaticClass()));
		if (AbilityComp)
		{
			FScriptDelegate handle;
			handle.BindUFunction(this, FName(TEXT("OnCommunicateInternal")));

			// if ability owner is server ROLE_Authority then register for server callback
			if (AbilityOwner->GetLocalRole() == ROLE_Authority)
			{
				AbilityComp->OnServerAbilityCommunicate.Add(handle);
			}
			
		}
			
	}

}

void AClientAuthorityAbilityActor::OnCommunicateInternal(AActor* AbilityOwner, FName NewMessage)
{
	OnCommunicate(NewMessage);

}

void AClientAuthorityAbilityActor::Communicate(FName NewMessage)
{
	if (AActor* AbilityOwner = GetOwner())
	{
		UAbilityComponent* AbilityComp = Cast<UAbilityComponent>(AbilityOwner->GetComponentByClass(UAbilityComponent::StaticClass()));
		if (AbilityComp)
		{
			if (AbilityOwner->GetLocalRole() < ROLE_Authority)
			{
				AbilityComp->Server_AbilityCommunicate(NewMessage);
			}

		}

	}
}

// Called every frame
void AClientAuthorityAbilityActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AClientAuthorityAbilityActor::IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const
{
	if (RealViewer)
	{
		const AController* RealViewerController = Cast<AController>(RealViewer) ? Cast<AController>(RealViewer) : Cast<APawn>(RealViewer->GetOwner()) ? Cast<APawn>(RealViewer->GetOwner())->GetController() : RealViewer->GetInstigatorController();
		const AController* OwnerController = Cast<APawn>(GetOwner()) ? Cast<APawn>(GetOwner())->GetController() : RealViewer->GetInstigatorController();

		if (RealViewerController == OwnerController)
		{
			return false;

		}
	}
	return Super::IsNetRelevantFor(RealViewer, ViewTarget, SrcLocation);
}

void AClientAuthorityAbilityActor::OnRep_Instigator()
{

	FScriptDelegate handle;
	handle.BindUFunction(this, FName(TEXT("OnCommunicateInternal")));
	// if the ability is someone else then register multicast callback.
	if (GetInstigatorController())
	{
		UAbilityComponent* AbilityComp = Cast<UAbilityComponent>(GetInstigatorController()->GetComponentByClass(UAbilityComponent::StaticClass()));
		if (AbilityComp)
		{
			AbilityComp->OnMulticastAbilityCommunicate.Add(handle);
		}
	}

	Super::OnRep_Instigator();
	K2_OnRepInstigator();

}
