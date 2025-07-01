// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Components/ActorComponent.h>
#include "AnimTraceListenerComponent.generated.h"

// Used for detecting gameplay hits from the Animation tracing system
UCLASS(ClassGroup = (AnimTracing), meta = (BlueprintSpawnableComponent))
class  UAnimTraceListenerComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()

		// Sets default values for this component's properties
		UAnimTraceListenerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Called when the animation system starts tracings
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnTraceStart, USkeletalMeshComponent*, MeshComponent, AActor*, ActorOwner, FName, FirstSocketName, FName, SecondSocketName);
	UPROPERTY(BlueprintAssignable)
		FOnTraceStart OnTraceStart;

	// Called on the owning actor when the animation tracing detects that the owning actor hit something
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnTraceHitActor, AActor*, ActorOwner, FName, FirstSocketName, FName, SecondSocketName, FHitResult, TraceHit, FVector, StrikeDirection, AActor*, HitActor);
	UPROPERTY(BlueprintAssignable)
		FOnTraceHitActor OnTraceHitActor;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnTraceEnd, USkeletalMeshComponent*, MeshComponent, AActor*, ActorOwner, FName, FirstSocketName, FName, SecondSocketName);
	UPROPERTY(BlueprintAssignable)
		FOnTraceEnd OnTraceEnd;

	// Called on the owning actor when the animation tracing detects that the owning actor hit something
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnServerOwnerHitActor, AActor*, Owner, FHitResult, TraceHit, FVector, StrikeDirection, AActor*, HitActor);
	UPROPERTY(BlueprintAssignable)
		FOnServerOwnerHitActor OnServerOwnerHitActor;

	// Called on the owning actor when the animation tracing detects that the owning actor hit something
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnClientOwnerHitActor, AActor*, Owner, FHitResult, TraceHit, FVector, StrikeDirection, AActor*, HitActor);
	UPROPERTY(BlueprintAssignable)
		FOnClientOwnerHitActor OnClientOwnerHitActor;

};
