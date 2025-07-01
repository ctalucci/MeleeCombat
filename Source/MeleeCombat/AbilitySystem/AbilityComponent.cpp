// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilityComponent.h"
#include "MeleeCombat/MeleeCombat.h"
#include <Net/UnrealNetwork.h>
#include "ClientAuthorityAbilityActor.h"
#include <Engine/World.h>
#include <GameFramework/Controller.h>
#include <GameFramework/Pawn.h>

// Sets default values for this component's properties
UAbilityComponent::UAbilityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}

// Called every frame
void UAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UAbilityComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{

	DOREPLIFETIME_CONDITION(UAbilityComponent, ClientAuthorityAbility, COND_SkipOwner);

	DOREPLIFETIME(UAbilityComponent, ServerAuthorityAbility);

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}



void UAbilityComponent::StartAbility(TSubclassOf<AActor> AbilityClass, AActor*& ActivatedAbility)
{
	if (AbilityClass && GetWorld() && GetOwner())
	{

	//	if (!ShouldStartAbility(AbilityClass))
		{
	//		return false;
		}

		EndAbility();

		FActorSpawnParameters NewActorParameters;
		NewActorParameters.Owner = GetOwner();
		NewActorParameters.Instigator = Cast<APawn>(GetOwner());

		bool bIsClientAuthorityActorClass = false;

		if (AActor* ActorCDO = Cast<AActor>(AbilityClass->GetDefaultObject()))
		{
			if (APawn* Pawn = Cast<APawn>(GetOwner()))
			{
				ActorCDO->SetOwner(GetOwner());
				bIsClientAuthorityActorClass = !ActorCDO->IsNetRelevantFor(Pawn->GetController(), GetOwner(), GetOwner()->GetActorLocation());
			}

		}

		FScriptDelegate handle;
		handle.BindUFunction(this, FName(TEXT("EndAbility")));
		
		if (bIsClientAuthorityActorClass)
		{
			ServerAuthorityAbility = nullptr;
			ClientAuthorityAbility = GetWorld()->SpawnActor<AActor>(AbilityClass, GetOwner()->GetActorTransform(), NewActorParameters);
			ClientAuthorityAbility->OnDestroyed.Add(handle);
		}
		else
		{
			if (GetOwner()->GetLocalRole() == ROLE_Authority)
			{
				ClientAuthorityAbility = nullptr;
				ServerAuthorityAbility = GetWorld()->SpawnActor<AActor>(AbilityClass, GetOwner()->GetActorTransform(), NewActorParameters);
				ServerAuthorityAbility->OnDestroyed.Add(handle);
			}
			
		}

		if (GetOwner()->GetLocalRole() < ROLE_Authority)
		{
			Server_StartAbility(AbilityClass);
		}
		
		ActivatedAbility = GetCurrentAbility();
		OnStartAbility.Broadcast(GetOwner(), GetCurrentAbility());
	}
}


void UAbilityComponent::EndAbility()
{
	OnEndAbility.Broadcast(GetOwner(), GetCurrentAbility());
	if (ClientAuthorityAbility)
	{
		ClientAuthorityAbility->Destroy();
		ClientAuthorityAbility = nullptr;

		if (GetOwnerRole() == ROLE_AutonomousProxy )
		{
			Server_EndAbility();
		}
	}

	if (ServerAuthorityAbility)
	{
		if (GetOwnerRole() == ROLE_AutonomousProxy)
		{
			Server_EndAbility();
		}
		else
		{
			ServerAuthorityAbility->Destroy();
			ServerAuthorityAbility = nullptr;
		}
	}
}

void UAbilityComponent::OnRep_Ability(AActor* PreviousAbility)
{

	if (GetCurrentAbility())
	{
		OnStartAbility.Broadcast(GetOwner(), GetCurrentAbility());
	}
	else
	{
		ServerAuthorityAbility = nullptr;
		ClientAuthorityAbility = nullptr;
	}

	if (PreviousAbility)
	{
		OnEndAbility.Broadcast(GetOwner(), GetCurrentAbility());
	}

}

void UAbilityComponent::Multicast_AbilityCommunicate_Implementation(FName Message)
{
	OnMulticastAbilityCommunicate.Broadcast(GetOwner(), Message);
}

bool UAbilityComponent::Server_AbilityCommunicate_Validate(FName Message)
{
	return true;
}

void UAbilityComponent::Server_AbilityCommunicate_Implementation(FName Message)
{
	OnServerAbilityCommunicate.Broadcast(GetOwner(), Message);
	Multicast_AbilityCommunicate(Message);
}




void UAbilityComponent::Server_StartAbility_Implementation(TSubclassOf<AActor> AbilityClass)
{
	AActor* CurrentAbility = nullptr;
	StartAbility(AbilityClass, CurrentAbility);
}

bool UAbilityComponent::Server_StartAbility_Validate(TSubclassOf<AActor> AbilityClass)
{
	return true;
}



void UAbilityComponent::Server_EndAbility_Implementation()
{
	EndAbility();
}

bool UAbilityComponent::Server_EndAbility_Validate()
{
	return true;
}

AActor*  UAbilityComponent::GetCurrentAbility() const
{
	return (ClientAuthorityAbility ? ClientAuthorityAbility : ServerAuthorityAbility);
}


