// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Components/ActorComponent.h>
#include "AbilityComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MELEECOMBAT_API UAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAbilityComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


	UPROPERTY(Transient, ReplicatedUsing = OnRep_Ability)
		AActor* ClientAuthorityAbility;

	UPROPERTY(Transient, ReplicatedUsing = OnRep_Ability)
		AActor* ServerAuthorityAbility;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = Ability, meta = (DeterminesOutputType = "AbilityClass", DynamicOutputParam = "ActivatedAbility"))
		void StartAbility(TSubclassOf<AActor> AbilityClass, AActor*& ActivatedAbility);

	UFUNCTION(BlueprintCallable, Category = Ability)
		void EndAbility();


	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Ability)
		AActor* GetCurrentAbility() const;

	
	UFUNCTION()
		void OnRep_Ability(AActor* PreviousAbility);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_StartAbility(TSubclassOf<AActor> AbilityClass);


	UFUNCTION(Server, Reliable, WithValidation)
		void Server_EndAbility();




	// Called when server detects ability has been started
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStartAbility, AActor*, Owner, AActor*, Ability);
	UPROPERTY(BlueprintAssignable)
		FOnStartAbility OnStartAbility;


	// Called on the owning actor when the animation tracing detects that the owning actor hit something
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEndAbility, AActor*, Owner, AActor*, Ability);
	UPROPERTY(BlueprintAssignable)
		FOnEndAbility OnEndAbility;

	// Called on the owning actor when the animation tracing detects that the owning actor hit something
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRequestStartAbility, AActor*, Owner, TSubclassOf<AActor>, AbilityClass);
	UPROPERTY(BlueprintAssignable)
	FOnRequestStartAbility OnRequestStartAbility;
	;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnServerAbilityCommunicate, AActor*, Owner, FName, Message);
	FOnServerAbilityCommunicate OnServerAbilityCommunicate;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMulticastAbilityCommunicate, AActor*, Owner, FName, Message);
	FOnMulticastAbilityCommunicate OnMulticastAbilityCommunicate;


	UFUNCTION(Server, Reliable, WithValidation)
		void Server_AbilityCommunicate(FName Message);

	UFUNCTION(NetMulticast, Reliable)
		void Multicast_AbilityCommunicate(FName Message);

};


