// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <GameFramework/Actor.h>
#include "ClientAuthorityAbilityActor.generated.h"


// Ability Actor which when spawned on the server by an owning controller won't replicate back to that owning controller
UCLASS()
class MELEECOMBAT_API AClientAuthorityAbilityActor : public AActor
{
	GENERATED_UCLASS_BODY()
	
public:	
	// Sets default values for this actor's properties
	AClientAuthorityAbilityActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnCommunicateInternal(AActor* AbilityOwner, FName NewMessage);

	UFUNCTION(BlueprintImplementableEvent, Category = Ability)
		void OnCommunicate(FName NewMessage);


	UFUNCTION(BlueprintCallable, Category = Ability)
		void Communicate(FName NewMessage);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual bool IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const override;

	virtual void OnRep_Instigator() override;

	UFUNCTION(BlueprintImplementableEvent, category = Ability)
		void K2_OnRepInstigator();
	
};
