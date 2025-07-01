// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Components/ActorComponent.h>
#include "AnimTraceListenerComponent.h"
#include <Curves/CurveFloat.h>
#include "AnimTraceSpecComponent.generated.h"




UCLASS( ClassGroup=(AnimTracing), meta=(BlueprintSpawnableComponent) )
class UAnimTraceSpecComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()


protected:
	// Called when the game starts
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UAnimTraceListenerComponent* LocalTraceListener;

	void GetCandidateTraceListenerSystems(TArray<UAnimTraceListenerComponent*> & Components);

	/*List of actors that the tracing will ignore*/
	UPROPERTY(Transient, BlueprintReadWrite, Category = Tracing)
		TArray<AActor*> TraceIgnores;

	/*I need some place to save previous mesh component to World for calculating previous bone locations
	I'm wondering if there is a better way to do this...
	*/
	UPROPERTY(Transient)
		FTransform LastMeshComponentToWorld;

	UPROPERTY(Transient)
		float LastEvaluationTime;

	/*Trace channel for hit detection, make sure that the volume you want to hit is set to Blocking*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Tracing)
		TEnumAsByte<ETraceTypeQuery>  CollisionTraceType;

	UFUNCTION(BlueprintCallable, BlueprintPure, category = Tracing)
		virtual FRotator GetProceduralAimRotation() const;

	UPROPERTY(EditDefaultsOnly, Category = Tracing)
		FName ProceduralAimBone;


	// Add a float curve here to alter aim pitch changes
	UPROPERTY(EditDefaultsOnly, Category = Tracing)
		UCurveFloat* AimPitchCurve;

	// If you want to dampen or increae pitch aiming while the trace is active
	UPROPERTY(EditDefaultsOnly, Category = Tracing)
		float AimModiferWhileTraceActive;

	UPROPERTY(EditDefaultsOnly, Category = Tracing)
		bool bEnableDebugDraw;

	// Don't hit an actor more then once in the same trace window, if you set this to false then you need to manage the TraceIgnores Array yourself.
	UPROPERTY(EditDefaultsOnly, Category = Tracing)
		bool bShouldIgnoreHitActors;


	UPROPERTY(Transient, BlueprintReadOnly, Category = Tracing)
		bool bIsTracing;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Tracing)
		int32 ActiveTraceCount;

	UFUNCTION()
		virtual void OnTraceStartDelegate(USkeletalMeshComponent* MeshComponent, AActor* ActorOwner, FName FirstSocketName, FName SecondSocketName);

	UFUNCTION()
		virtual void OnTraceHitActorDelegate(AActor* ActorOwner, FName FirstSocketName, FName SecondSocketName, FHitResult TraceHit, FVector StrikeDirection, AActor* HitActor);

	UFUNCTION()
		virtual void OnTraceEndDelegate(USkeletalMeshComponent* MeshComponent, AActor* ActorOwner, FName FirstSocketName, FName SecondSocketName);

	/* Replication Functions */ 
	
	/* If we want other players to know the details of an owning clients tracing simulation, first send to server*/
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_OnTraceHitActor(FHitResult TraceHit, FVector StrikeDirection);

	/* If we want other players to know the details of an owning clients tracing simulation, then server send to Clients*/
	UFUNCTION(NetMulticast, Reliable)
		void Multicast_OnTraceHitActor(FHitResult TraceHit, FVector StrikeDirection);


	UFUNCTION(BlueprintCallable, Category = Tracing)
		void AddActorToTraceListeners(AActor* ActorToAdd);

	private:

		/*List of Actors with trace listeners */
		UPROPERTY()
		TArray<TWeakObjectPtr<AActor>> ActiveTraceListenerActors;



};
