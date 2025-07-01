// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimTraceSpecComponent.h"
#include "AnimStateTracing.h"
#include <GameFramework/Pawn.h>



// Sets default values for this component's properties
UAnimTraceSpecComponent::UAnimTraceSpecComponent(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	CollisionTraceType = ETraceTypeQuery::TraceTypeQuery1;
	AimModiferWhileTraceActive = .5f;
	bEnableDebugDraw = false;
	bShouldIgnoreHitActors = true;

	FName TraceListenerName(TEXT("LocalTraceListener"));

	LocalTraceListener=CreateDefaultSubobject<UAnimTraceListenerComponent>(TraceListenerName);

	if (LocalTraceListener)
	{
		FScriptDelegate handle;
		handle.BindUFunction(this, FName(TEXT("OnTraceStartDelegate")));

		LocalTraceListener->OnTraceStart.Add(handle);

		handle.BindUFunction(this, FName(TEXT("OnTraceEndDelegate")));
		LocalTraceListener->OnTraceEnd.Add(handle);

		handle.BindUFunction(this, FName(TEXT("OnTraceHitActorDelegate")));
		LocalTraceListener->OnTraceHitActor.Add(handle);
	}

	ActiveTraceCount = 0;

}


// Called when the game starts
void UAnimTraceSpecComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void UAnimTraceSpecComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}




void UAnimTraceSpecComponent::GetCandidateTraceListenerSystems(TArray<UAnimTraceListenerComponent*> &Components)
{
	if (GetOwner())
	{
		GetOwner()->GetComponents(Components);
	}

	for (auto ActiveTraceListenersActor : ActiveTraceListenerActors)
	{
		if (ActiveTraceListenersActor.IsValid())
		{
			TArray<UAnimTraceListenerComponent*> MoreComponents;
			ActiveTraceListenersActor->GetComponents(MoreComponents);
			Components.Append(MoreComponents);
		}
	}

}

FRotator UAnimTraceSpecComponent::GetProceduralAimRotation() const
{

	if (APawn* OwnerPawn = Cast<APawn>(GetOwner()))
	{
		// Set aim Pitch and Yaw
		const FVector AimDirWS = OwnerPawn->GetBaseAimRotation().Vector();
		const FVector AimDirLS = OwnerPawn->ActorToWorld().InverseTransformVectorNoScale(AimDirWS);
		const FRotator Aims = AimDirLS.Rotation();
		float Pitch = Aims.Pitch * -1;

		if (AimPitchCurve)
		{
			Pitch = (AimPitchCurve->GetFloatValue(Pitch));
		}

		return FRotator(0, 0, Pitch*AimModiferWhileTraceActive);

	}
	return FRotator(0, 0, 0);
}

void UAnimTraceSpecComponent::OnTraceStartDelegate(USkeletalMeshComponent* MeshComponent, AActor* ActorOwner, FName FirstSocketName, FName SecondSocketName)
{
	AimModiferWhileTraceActive = .5f;
	TraceIgnores.Empty();
	bIsTracing = true;
	ActiveTraceCount++;
}

void UAnimTraceSpecComponent::OnTraceEndDelegate(USkeletalMeshComponent* MeshComponent, AActor* ActorOwner, FName FirstSocketName, FName SecondSocketName)
{
	AimModiferWhileTraceActive = 1.0f;
	TraceIgnores.Empty();
	ActiveTraceCount--;
	if (bIsTracing && ActiveTraceCount <= 0)
	{
		bIsTracing = false;
	}
		
}

void UAnimTraceSpecComponent::AddActorToTraceListeners(AActor * ActorToAdd)
{
	ActiveTraceListenerActors.Add(ActorToAdd);

}

void UAnimTraceSpecComponent::Multicast_OnTraceHitActor_Implementation(FHitResult TraceHit, FVector StrikeDirection)
{
	if (bShouldIgnoreHitActors)
	{
		TraceIgnores.Add(TraceHit.GetActor());
	}
	
	TArray<UAnimTraceListenerComponent*> TraceListeners;
	GetCandidateTraceListenerSystems(TraceListeners);

	for (auto TraceListener : TraceListeners)
	{
		TraceListener->OnClientOwnerHitActor.Broadcast(GetOwner(), TraceHit, StrikeDirection, TraceHit.GetActor());
	}
}

void UAnimTraceSpecComponent::Server_OnTraceHitActor_Implementation(FHitResult TraceHit, FVector StrikeDirection)
{
	if (bShouldIgnoreHitActors)
	{
		TraceIgnores.Add(TraceHit.GetActor());
	}
	
	TArray<UAnimTraceListenerComponent*> TraceListeners;
	GetCandidateTraceListenerSystems(TraceListeners);

	for (auto TraceListener : TraceListeners)
	{
		TraceListener->OnServerOwnerHitActor.Broadcast(GetOwner(), TraceHit, StrikeDirection, TraceHit.GetActor());
	}
}

bool UAnimTraceSpecComponent::Server_OnTraceHitActor_Validate(FHitResult TraceHit, FVector StrikeDirection)
{
	return true;
}

void UAnimTraceSpecComponent::OnTraceHitActorDelegate(AActor* ActorOwner, FName FirstSocketName, FName SecondSocketName, FHitResult TraceHit, FVector StrikeDirection, AActor* HitActor)
{
	if (bShouldIgnoreHitActors)
	{
		TraceIgnores.Add(TraceHit.GetActor());
	}

	// only send to server if we our owner is locally controlled.
	const bool bLocalPawnOwner = Cast<APawn>(GetOwner()) && Cast<APawn>(GetOwner())->IsLocallyControlled();
	const bool bLocalAuthority = GetOwner()->GetLocalRole() == ROLE_Authority && GetNetMode() == NM_Client;
	if (bLocalPawnOwner)
	{
		Server_OnTraceHitActor(TraceHit, StrikeDirection);
	}

}