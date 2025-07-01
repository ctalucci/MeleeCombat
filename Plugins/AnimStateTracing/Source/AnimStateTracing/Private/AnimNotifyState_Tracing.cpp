// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNotifyState_Tracing.h"
#include "AnimStateTracing.h"
#include <Animation/AnimInstance.h>
#include <Engine/SkeletalMeshSocket.h>
#include <Animation/AnimNotifies/AnimNotify.h>
#include <DrawDebugHelpers.h>
#include "AnimTraceListenerComponent.h"
#include <AnimationRuntime.h>
#include "AnimTraceSpecComponent.h"
#include <GameFramework/Actor.h>
#include "Engine/Engine.h"
#include <Kismet/KismetSystemLibrary.h>
#include <Animation/AnimSlotEvaluationPose.h>



typedef TInlineComponentArray<UAnimTraceSpecComponent*, 8> AnimTraceSpecComponentArray;

static void GetCandidateTraceSpecSystems(USkeletalMeshComponent& MeshComp, AnimTraceSpecComponentArray& Components)
{
	if (AActor* Owner = MeshComp.GetOwner())
	{
		Owner->GetComponents(Components);
	}
}


static void GetCandidateTraceListenerSystems(USkeletalMeshComponent& MeshComp, TArray<UAnimTraceListenerComponent*> &Components)
{
	AnimTraceSpecComponentArray TraceSpecs;
	GetCandidateTraceSpecSystems(MeshComp, TraceSpecs);
	if (TraceSpecs.Num() > 0)
	{
		TraceSpecs[0]->GetCandidateTraceListenerSystems(Components);
	}

}

UAnimNotifyState_Tracing::UAnimNotifyState_Tracing(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

#if WITH_EDITORONLY_DATA
	NotifyColor = FColor(200, 200, 255, 255);
#endif // WITH_EDITORONLY_DATA

	bIsNativeBranchingPoint = true;
}

bool UAnimNotifyState_Tracing::HasValidBoneNames(USkeletalMeshComponent* MeshComp)  const
{
	return MeshComp && (MeshComp->GetBoneIndex(FirstSocketName) != INDEX_NONE || MeshComp->GetSocketByName(FirstSocketName)) && (MeshComp->GetBoneIndex(SecondSocketName) != INDEX_NONE || MeshComp->GetSocketByName(SecondSocketName));
}


void UAnimNotifyState_Tracing::BranchingPointNotifyBegin(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	USkeletalMeshComponent* MeshComp = BranchingPointPayload.SkelMeshComponent;
	UAnimSequenceBase* Animation = BranchingPointPayload.SequenceAsset;

	if (HasValidBoneNames(MeshComp))
	{
		const float CurrentTimePosition = BranchingPointPayload.NotifyEvent->GetTriggerTime();

		AnimTraceSpecComponentArray TraceSpecs;
		GetCandidateTraceSpecSystems(*MeshComp, TraceSpecs);
		if (TraceSpecs.Num() > 0)
		{
			TraceSpecs[0]->LastMeshComponentToWorld = MeshComp->GetComponentTransform();
			TraceSpecs[0]->LastEvaluationTime = CurrentTimePosition;
			TraceSpecs[0]->bIsTracing = true;
		}

		TArray<UAnimTraceListenerComponent*> TraceListeners;
		GetCandidateTraceListenerSystems(*MeshComp, TraceListeners);
		for (auto TraceListener : TraceListeners)
		{
			if (!TraceListener) { continue; }
			TraceListener->OnTraceStart.Broadcast(MeshComp, MeshComp->GetOwner(), FirstSocketName, SecondSocketName);
		}
	}

	Super::BranchingPointNotifyBegin(BranchingPointPayload);
}


void UAnimNotifyState_Tracing::BranchingPointNotifyTick(FBranchingPointNotifyPayload& BranchingPointPayload, float FrameDeltaTime)
{
	USkeletalMeshComponent* MeshComp = BranchingPointPayload.SkelMeshComponent;
	UAnimSequenceBase* Animation = BranchingPointPayload.SequenceAsset;

	if (HasValidBoneNames(MeshComp) && MeshComp->GetAnimInstance() && MeshComp->GetAnimInstance()->GetActiveMontageInstance())
	{
		const float CurrentTimePosition = BranchingPointPayload.SkelMeshComponent->GetAnimInstance()->GetActiveMontageInstance()->GetPosition();
		FTransform PreviousTickTransform = MeshComp->GetComponentTransform();
		float LastTickTime = CurrentTimePosition;

		AnimTraceSpecComponentArray TraceSpecs;
		GetCandidateTraceSpecSystems(*MeshComp, TraceSpecs);
		bool bCanStartTracing = bEnableDebugDraw;
		if (TraceSpecs.Num() > 0)
		{
			LastTickTime = TraceSpecs[0]->LastEvaluationTime;
			PreviousTickTransform = TraceSpecs[0]->LastMeshComponentToWorld;
			TraceSpecs[0]->LastMeshComponentToWorld = MeshComp->GetComponentTransform();
			TraceSpecs[0]->LastEvaluationTime = CurrentTimePosition;
			bCanStartTracing = TraceSpecs[0]->bIsTracing;
		}
		if (bCanStartTracing)
		{
			FillInTraces(CurrentTimePosition, LastTickTime, CurrentTimePosition, LastTickTime, MeshComp, Animation, PreviousTickTransform);
		}

	}

	Super::BranchingPointNotifyTick(BranchingPointPayload, FrameDeltaTime);

}

void UAnimNotifyState_Tracing::BranchingPointNotifyEnd(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	USkeletalMeshComponent* MeshComp = BranchingPointPayload.SkelMeshComponent;
	UAnimSequenceBase* Animation = BranchingPointPayload.SequenceAsset;

	if (HasValidBoneNames(MeshComp))
	{
		const float CurrentTimePosition = BranchingPointPayload.NotifyEvent->GetDuration() + BranchingPointPayload.NotifyEvent->GetTriggerTime();
		float LastTickTime = BranchingPointPayload.NotifyEvent->GetTriggerTime();
		FTransform PreviousTickTransform = MeshComp->GetComponentTransform();


		AnimTraceSpecComponentArray TraceSpecs;
		GetCandidateTraceSpecSystems(*MeshComp, TraceSpecs);
		bool bCanStartTracing = bEnableDebugDraw;
		if (TraceSpecs.Num() > 0)
		{
			LastTickTime = TraceSpecs[0]->LastEvaluationTime;
			PreviousTickTransform = TraceSpecs[0]->LastMeshComponentToWorld;
			TraceSpecs[0]->LastMeshComponentToWorld = MeshComp->GetComponentTransform();
			TraceSpecs[0]->LastEvaluationTime = CurrentTimePosition;
			bCanStartTracing = TraceSpecs[0]->bIsTracing;
		}

		if (bCanStartTracing)
		{
			FillInTraces(CurrentTimePosition, LastTickTime, CurrentTimePosition, LastTickTime, MeshComp, Animation, PreviousTickTransform);
		}

		TArray<UAnimTraceListenerComponent*> TraceListeners;
		GetCandidateTraceListenerSystems(*MeshComp, TraceListeners);

		for (auto TraceListener : TraceListeners)
		{
			if (!TraceListener) { continue; }
			TraceListener->OnTraceEnd.Broadcast(MeshComp, MeshComp->GetOwner(), FirstSocketName, SecondSocketName);
		}

	}

	Super::BranchingPointNotifyEnd(BranchingPointPayload);
}


FVector UAnimNotifyState_Tracing::GetStrikeVector(float EvaluationTime, float PreviousEvaluationTime, FVector StartLocation, FVector EndLocation, FVector PreviousStartLocation, FVector PreviousEndLocation)
{
	const FVector ThisMidpoint = ((EndLocation - StartLocation) / 2) + StartLocation;
	const FVector LastMidpoint = ((PreviousEndLocation - PreviousStartLocation) / 2) + PreviousStartLocation;
	return 	(ThisMidpoint - LastMidpoint).GetSafeNormal() * FMath::Sign(EvaluationTime - PreviousEvaluationTime);
}


bool UAnimNotifyState_Tracing::ShouldDrawMoreTraces(float EvaluationTime, float PreviousEvaluationTime, FVector StartLocation, FVector EndLocation, FVector PreviousStartLocation, FVector PreviousEndLocation)
{
	const float EndDistance = (EndLocation - PreviousEndLocation).Size();
	const float MaxDistance = FMath::Max(0.0f, EndDistance);
	return 	MaxDistance > MinTraceDistance && MinTraceDistance > 0;
}

void UAnimNotifyState_Tracing::FillInTraces(float MaxTime, float MinTime, float EvaluationTime, float PreviousEvaluationTime, USkeletalMeshComponent* MeshComp, UAnimSequenceBase * Animation, const FTransform& PreviousMeshWorldTransform)
{

	FVector StartLocation;
	FVector EndLocation;

	FVector LastTraceStart;
	FVector LastTraceEnd;

	GetStartEndLocationForBones(EvaluationTime, MeshComp, Animation, MeshComp->GetComponentTransform(), StartLocation, EndLocation);

	GetStartEndLocationForBones(PreviousEvaluationTime, MeshComp, Animation, PreviousMeshWorldTransform, LastTraceStart, LastTraceEnd);

	const FVector StrikeDirection = GetStrikeVector(EvaluationTime, PreviousEvaluationTime, StartLocation, EndLocation, LastTraceStart, LastTraceEnd);

	DrawCapsuleTrace(MeshComp, StartLocation, EndLocation, StrikeDirection);

	if (!bShouldInterpolateTraces || !ShouldDrawMoreTraces(EvaluationTime, PreviousEvaluationTime, StartLocation, EndLocation, LastTraceStart, LastTraceEnd) || EvaluationTime == PreviousEvaluationTime) { return; }

	const float NewEvalDistance = (EvaluationTime - PreviousEvaluationTime) / 2.0f;

	if (EvaluationTime + NewEvalDistance < MaxTime)
	{
		FillInTraces(MaxTime, MinTime, EvaluationTime + NewEvalDistance, EvaluationTime, MeshComp, Animation, MeshComp->GetComponentTransform());
	}

	if (EvaluationTime - NewEvalDistance > MinTime)
	{
		FillInTraces(MaxTime, MinTime, EvaluationTime - NewEvalDistance, EvaluationTime, MeshComp, Animation, MeshComp->GetComponentTransform());
	}

}


void UAnimNotifyState_Tracing::DrawCapsuleTrace(USkeletalMeshComponent* MeshComp, const FVector StartLocation, const FVector EndLocation, const FVector StrikeDirection)
{
	UWorld* World = GEngine->GetWorldFromContextObjectChecked(MeshComp);
	if (World && MeshComp)
	{

		const FVector LineTrace = EndLocation - StartLocation;
		const FVector CapsuleCenter = ((LineTrace)*.5) + StartLocation;
		const FQuat Rot = FRotationMatrix::MakeFromZ(LineTrace).Rotator().Quaternion();
		const float HalfHeight = ((LineTrace).Size()) / 2;

		ETraceTypeQuery CollisionTraceType = ETraceTypeQuery::TraceTypeQuery1;
		bool bComponentEnableDebugDraw = bEnableDebugDraw;
		TArray<AActor*> Ignores;
		Ignores.Add(MeshComp->GetOwner());

		AnimTraceSpecComponentArray TraceSpec;
		GetCandidateTraceSpecSystems(*MeshComp, TraceSpec);
		if (TraceSpec.Num() > 0)
		{
			bComponentEnableDebugDraw = TraceSpec[0]->bEnableDebugDraw;
			CollisionTraceType = TraceSpec[0]->CollisionTraceType;
			Ignores.Append(TraceSpec[0]->TraceIgnores);
		}


		TArray<FHitResult> OutHits;

		const bool bShouldDrawTraces = bComponentEnableDebugDraw;

		EDrawDebugTrace::Type DebugTrace = bShouldDrawTraces ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

		//draw the strike vectors
		if (bShouldDrawTraces)
		{
			const FVector MidPoint = ((EndLocation - StartLocation) / 2) + StartLocation;
			const FVector StikeEnd = MidPoint + (StrikeDirection * 100);

			DrawDebugDirectionalArrow(World, MidPoint, StikeEnd, 10, FColor::Yellow, false, 5, 0, 0.0);
		}



		bool bFoundBlockingHit = UKismetSystemLibrary::SphereTraceMulti(World, StartLocation, EndLocation, CapsuleSize, CollisionTraceType, true, Ignores, DebugTrace, OutHits, true);

		// if we didn't find a hit on the forward tracing check reverse tracing in case StartLocation originated inside collision geometry
		if (!bFoundBlockingHit)
		{
			bFoundBlockingHit = UKismetSystemLibrary::SphereTraceMulti(World, StartLocation, EndLocation, CapsuleSize, CollisionTraceType, true, Ignores, DebugTrace, OutHits, true);
		}


		if (bFoundBlockingHit)
		{
			for (auto hit : OutHits)
			{

				//	 Draw the hits
				if (bShouldDrawTraces)
				{
					DrawDebugDirectionalArrow(World, hit.Location, hit.Location + (StrikeDirection * 100), 10, FColor::Cyan, false, 5, 0, 0.0);
				}


				TArray<UAnimTraceListenerComponent*> TraceListeners;
				GetCandidateTraceListenerSystems(*MeshComp, TraceListeners);

				for (auto TraceListener : TraceListeners)
				{
					if (!TraceListener) { continue; }
					TraceListener->OnTraceHitActor.Broadcast(MeshComp->GetOwner(), FirstSocketName, SecondSocketName, hit, StrikeDirection, hit.GetActor());
				}
				break; // Maybe handle multiple blocking hits ?
			}
		}

	}
}

void UAnimNotifyState_Tracing::GetStartEndLocationForBones(double EvaluationTime, USkeletalMeshComponent* MeshComp, UAnimSequenceBase * Animation, const FTransform& WorldTransform, FVector & StartLocation, FVector & EndLocation)
{
	if (!Animation) { return; }

	FSlotEvaluationPose NewPose(0, Animation->GetAdditiveAnimType());

	// Bone array has to be allocated prior to calling GetPoseFromAnimTrack

	if (MeshComp && MeshComp->GetAnimInstance())
	{
		NewPose.Pose.SetBoneContainer(&(MeshComp->GetAnimInstance()->GetRequiredBones()));
		NewPose.Curve.InitFrom(MeshComp->GetAnimInstance()->GetRequiredBones());
	}

	if (UAnimMontage* Montage = Cast<UAnimMontage>(Animation))
	{

		TArray<UAnimSequenceBase*> AnimSequences;

		for (auto AnimTrack : Montage->SlotAnimTracks)
		{
			for (auto AnimSegment : AnimTrack.AnimTrack.AnimSegments)
			{
				AnimSequences.Add(AnimSegment.GetAnimReference());
			}
		}

		for (auto AnimSequence : AnimSequences)
		{
			if (UAnimSequence* Sequence = Cast<UAnimSequence>(AnimSequence))
			{
				UE::Anim::FStackAttributeContainer TempAttributes;
				FAnimationPoseData OutAnimationPoseData(NewPose.Pose, NewPose.Curve, TempAttributes);
				FAnimExtractContext OutAnimContext(EvaluationTime, Montage->HasRootMotion());
				Sequence->GetAnimationPose(OutAnimationPoseData, OutAnimContext);
				break;
			}
		}

	}
	else
	{
		NewPose.Pose.ResetToRefPose();
	}

	EvaluateAdditiveBoneRotation(MeshComp, MeshComp->GetAnimInstance(), NewPose.Pose);

	NewPose.Pose.NormalizeRotations();

	struct SocketAndIndex {
		FTransform Socket;
		int32 Index;
		bool Invalid()const { return Index == INDEX_NONE; }
	};

	auto GetBoneIndex = [MeshComp](const FName& BoneName)->SocketAndIndex
	{
		SocketAndIndex result;
		result.Index = MeshComp->GetBoneIndex(BoneName);
		if (result.Invalid()) {
			if (auto socket = MeshComp->GetSocketByName(BoneName))
			{
				result.Socket = FTransform(socket->RelativeRotation, socket->RelativeLocation, socket->RelativeScale);
				result.Index = MeshComp->GetBoneIndex(socket->BoneName);
			}

		}
		return result;
	};
	const auto StartSI = GetBoneIndex(FirstSocketName);
	const auto EndSI = GetBoneIndex(SecondSocketName);
	if (StartSI.Invalid() || EndSI.Invalid())
	{
		return;
	}


	TArray<FTransform> Bones;
	Bones.SetNum(MeshComp->RequiredBones.Num());
	FillSpaceBases(MeshComp->RequiredBones, MeshComp->GetSkeletalMeshAsset(), NewPose.Pose.GetBones(), Bones);

	//set output


	StartLocation = (StartSI.Socket * ((Bones[StartSI.Index]) * (WorldTransform))).GetLocation();
	EndLocation = (EndSI.Socket * ((Bones[EndSI.Index]) * (WorldTransform))).GetLocation();
}


//TODO: just get the two bones we care about
void UAnimNotifyState_Tracing::FillSpaceBases(const TArray<FBoneIndexType>& RequiredBones, const USkeletalMesh* InSkeletalMesh, const TArray<FTransform, FAnimStackAllocator>& SourceAtoms, TArray<FTransform>& DestSpaceBases)
{
	if (!InSkeletalMesh)
	{
		return;
	}

	// right now all this does is populate DestSpaceBases
	check(InSkeletalMesh->GetRefSkeleton().GetNum() == SourceAtoms.Num());
	check(InSkeletalMesh->GetRefSkeleton().GetNum() == DestSpaceBases.Num());

	const int32 NumBones = SourceAtoms.Num();

#if (UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT)
	/** Keep track of which bones have been processed for fast look up */
	TArray<uint8> BoneProcessed;
	BoneProcessed.AddZeroed(NumBones);
#endif

	const FTransform* LocalTransformsData = SourceAtoms.GetData();
	FTransform* SpaceBasesData = DestSpaceBases.GetData();

	// First bone is always root bone, and it doesn't have a parent.
	{
		check(RequiredBones[0] == 0);
		DestSpaceBases[0] = SourceAtoms[0];

#if (UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT)
		// Mark bone as processed
		BoneProcessed[0] = 1;
#endif
	}

	for (int32 i = 1; i < RequiredBones.Num(); i++)
	{
		const int32 BoneIndex = RequiredBones[i];
		FTransform* SpaceBase = SpaceBasesData + BoneIndex;

		FPlatformMisc::Prefetch(SpaceBase);

#if (UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT)
		// Mark bone as processed
		BoneProcessed[BoneIndex] = 1;
#endif
		// For all bones below the root, final component-space transform is relative transform * component-space transform of parent.
		const int32 ParentIndex = InSkeletalMesh->GetRefSkeleton().GetParentIndex(BoneIndex);
		FTransform* ParentSpaceBase = SpaceBasesData + ParentIndex;
		FPlatformMisc::Prefetch(ParentSpaceBase);

#if (UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT)
		// Check the precondition that Parents occur before Children in the RequiredBones array.
		checkSlow(BoneProcessed[ParentIndex] == 1);
#endif
		FTransform::Multiply(SpaceBase, LocalTransformsData + BoneIndex, ParentSpaceBase);

		checkSlow(SpaceBase->IsRotationNormalized());
		checkSlow(!SpaceBase->ContainsNaN());
	}

	/**
	* Normalize rotations.
	* We want to remove any loss of precision due to accumulation of error.
	* i.e. A componentSpace transform is the accumulation of all of its local space parents. The further down the chain, the greater the error.
	* SpaceBases are used by external systems, we feed this to PhysX, send this to gameplay through bone and socket queries, etc.
	* So this is a good place to make sure all transforms are normalized.
	*/
	FAnimationRuntime::NormalizeRotations(DestSpaceBases);
}

FRotator UAnimNotifyState_Tracing::GetDefaultOwnerPawnAimRotation(USkeletalMeshComponent* MeshComp)
{
	if (MeshComp)
	{

		if (APawn* OwnerPawn = Cast<APawn>(MeshComp->GetOwner()))
		{

			// Set aim Pitch and Yaw
			const FVector AimDirWS = OwnerPawn->GetBaseAimRotation().Vector();
			const FVector AimDirLS = OwnerPawn->ActorToWorld().InverseTransformVectorNoScale(AimDirWS);
			const FRotator Aims = AimDirLS.Rotation();
			float Pitch = Aims.Pitch * -1;
			return FRotator(0, 0, Pitch);
		}
	}
	return FRotator(0, 0, 0);
}


void UAnimNotifyState_Tracing::EvaluateAdditiveBoneRotation(USkeletalMeshComponent* MeshComp, UAnimInstance* Animation, FCompactPose& outPose)
{

	FCSPose<FCompactPose> OutMeshPose;
	OutMeshPose.InitPose(outPose);


	if (MeshComp)
	{

		AnimTraceSpecComponentArray TraceSpecs;
		GetCandidateTraceSpecSystems(*MeshComp, TraceSpecs);
		if (TraceSpecs.Num() > 0)
		{
			const FBoneContainer& BoneContainer = OutMeshPose.GetPose().GetBoneContainer();

			FBoneReference BoneToModify = FBoneReference(TraceSpecs[0]->ProceduralAimBone);
			BoneToModify.Initialize(BoneContainer);

			FCompactPoseBoneIndex CompactPoseBoneToModify = BoneToModify.GetCompactPoseIndex(BoneContainer);
			FTransform NewBoneTM = OutMeshPose.GetComponentSpaceTransform(CompactPoseBoneToModify);
			FTransform ComponentTransform = MeshComp->GetComponentTransform();

			// Convert to Bone Space.
			FAnimationRuntime::ConvertCSTransformToBoneSpace(ComponentTransform, OutMeshPose, NewBoneTM, CompactPoseBoneToModify, EBoneControlSpace::BCS_ComponentSpace);

			const FQuat BoneQuat(TraceSpecs[0]->GetProceduralAimRotation());
			NewBoneTM.SetRotation(BoneQuat * NewBoneTM.GetRotation());

			// Convert back to Component Space.
			FAnimationRuntime::ConvertBoneSpaceTransformToCS(ComponentTransform, OutMeshPose, NewBoneTM, CompactPoseBoneToModify, EBoneControlSpace::BCS_ComponentSpace);

			TArray<FBoneTransform> BoneTransforms;
			BoneTransforms.Add(FBoneTransform(BoneToModify.GetCompactPoseIndex(BoneContainer), NewBoneTM));
			if (BoneTransforms.Num() > 0)
			{
				OutMeshPose.LocalBlendCSBoneTransforms(BoneTransforms, 1.0f);
			}

			
			FCSPose<FCompactPose>::ConvertComponentPosesToLocalPosesSafe(OutMeshPose, outPose);

		}
	}
}
