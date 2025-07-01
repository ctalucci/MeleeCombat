// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimBlueprintFunctionLibrary.h"
#include "AnimStateTracing.h"
#include "AnimNotifyState_Tracing.h"
#include <Animation/AnimMontage.h>
#include <Animation/AnimInstance.h>


float UAnimBlueprintFunctionLibrary::GetDirectionYaw(APawn* Pawn)
{
	// speed and direction
	if (!Pawn) { return 0.f; }

	const FVector Velocity = Pawn->GetVelocity();
	const float Speed = Velocity.Size();

	if (Speed >= 0)
	{
		const FQuat VelQuat = FQuat(Velocity.Rotation());
		const FQuat FacingQuat = FQuat(Pawn->GetActorRotation()).Inverse();
		const float DirectionYaw = FRotator(VelQuat * FacingQuat).Yaw;

		return DirectionYaw >= 180 ? DirectionYaw - 360 : DirectionYaw;
	}
	return 0.f;
}




void UAnimBlueprintFunctionLibrary::PlayMontageSectionWithDuration(USkeletalMeshComponent* MeshComp, UAnimMontage* Montage, float Duration, FName SectionName)
{

	if (!Montage || Duration <= 0 || !MeshComp->GetAnimInstance())
	{
		return;
	}

	const float newPlayRate = ((Montage->GetSectionLength(Montage->GetSectionIndex(SectionName))) / Duration);

	if (MeshComp->GetAnimInstance()->Montage_IsPlaying(Montage))
	{
		MeshComp->GetAnimInstance()->Montage_SetPlayRate(Montage, newPlayRate);
		MeshComp->GetAnimInstance()->Montage_JumpToSection(SectionName, Montage);
	}
	else
	{
		MeshComp->GetAnimInstance()->Montage_Play(Montage, newPlayRate);
		MeshComp->GetAnimInstance()->Montage_JumpToSection(SectionName);
	}
}

void UAnimBlueprintFunctionLibrary::PlayMontageTracingSectionWithDuration(USkeletalMeshComponent* MeshComp, UAnimMontage* Montage, float Duration, ETraceSectionType::Type Section)
{
	if (!Montage || Duration <= 0 || !MeshComp->GetAnimInstance())
	{
		return;
	}

	float TriggerTime = 0.f;
	float TraceDuration = 0.f;
	for (FAnimNotifyEvent Notify : Montage->Notifies)
	{
		if (!Notify.NotifyStateClass) continue;

		if (Notify.NotifyStateClass->IsA(UAnimNotifyState_Tracing::StaticClass()))
		{
			TriggerTime = Notify.GetTriggerTime();
			TraceDuration = Notify.GetDuration();
		}
	}



	float newPlayRate = 1.0f;
	float StartTime = 0.f ;

	switch (Section)
	{
	case ETraceSectionType::BeforeTrace:
		newPlayRate = (TriggerTime / Duration) ;
		break;

	case ETraceSectionType::DuringTrace:
		newPlayRate = (TraceDuration / Duration) ;
		StartTime = TriggerTime;
		break;

	case ETraceSectionType::AfterTrace:
		StartTime = TraceDuration + TriggerTime;
		newPlayRate = ((Montage->GetPlayLength() - (StartTime)) / Duration) ;
		break;
	default:

		break;
	}

	if (MeshComp->GetAnimInstance()->Montage_IsPlaying(Montage))
	{
		MeshComp->GetAnimInstance()->Montage_SetPosition(Montage, StartTime );
		MeshComp->GetAnimInstance()->Montage_SetPlayRate(Montage, newPlayRate);
	}
	else
	{
		MeshComp->GetAnimInstance()->Montage_Play(Montage, newPlayRate, EMontagePlayReturnType::MontageLength, StartTime);
	}
	
}
