// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <Animation/AnimTypes.h>
#include <Animation/AnimNotifies/AnimNotifyState.h>
#include <BoneIndices.h>
#include <Engine/SkeletalMesh.h>
#include "AnimNotifyState_Tracing.generated.h"





/**
 * Does capsule Animation based tracing for a pair of bones. If animation speed is very fast, we will fill in the gaps, based on MinTraceDistance End Bone Distance from previous traces
 */


UCLASS(editinlinenew, Blueprintable, const, hidecategories = Object, collapsecategories, meta = (ShowWorldContextPin, DisplayName = "Tracing"))
class UAnimNotifyState_Tracing : public UAnimNotifyState
{
	GENERATED_UCLASS_BODY()

		/** Name of the first socket to start tracing from. */
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tracing)
		FName FirstSocketName;

	/** Name of the second socket to trace to. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tracing)
		FName SecondSocketName;

	/** If the distance between frames is larger then this we will try and interpolate between them, set to 0 for no interpolation**/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tracing)
		float MinTraceDistance = 25.0f;

	/* The size of the capsules we are tracing */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tracing)
		float CapsuleSize = 10.0f;

	/* Turn off/on the trace lines */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tracing)
		bool bEnableDebugDraw = 0;

	/** always turns off tracing if false*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tracing)
		bool bShouldInterpolateTraces = 1;


	bool HasValidBoneNames(USkeletalMeshComponent* MeshComp) const;
	/*AnimNotifyState Interface Begin*/
	virtual void BranchingPointNotifyBegin(FBranchingPointNotifyPayload& BranchingPointPayload) override;
	virtual void BranchingPointNotifyTick(FBranchingPointNotifyPayload& BranchingPointPayload, float FrameDeltaTime) override;
	virtual void BranchingPointNotifyEnd(FBranchingPointNotifyPayload& BranchingPointPayload) override;

	/*AnimNotifyState Interface End*/

protected:
	//Returns Start And End location for the two sockets
	UFUNCTION(BlueprintCallable, Category = Tracing)
		void GetStartEndLocationForBones(double RelativeTimePercentage, USkeletalMeshComponent* MeshComp, UAnimSequenceBase * Animation, const FTransform& WorldTransform, FVector & StartLocation, FVector & EndLocation);

	// expose these to blueprint?
	virtual	FVector GetStrikeVector(const float EvaluationTime, const float PreviousEvaluationTime, const FVector StartLocation, const FVector EndLocation, const FVector PreviousStartLocation, const FVector PreviousEndLocation) ;
	virtual	 bool ShouldDrawMoreTraces(const float CurrentEvaluationTime, const float PreviousEvaluationTime, const FVector StartLocation, const FVector EndLocation, const FVector PreviousStartLocation, const FVector PreviousEndLocation) ;


private:

	void FillSpaceBases(const TArray<FBoneIndexType>& RequiredBones, const USkeletalMesh* InSkeletalMesh, const TArray<FTransform, FAnimStackAllocator>& SourceAtoms, TArray<FTransform>& DestSpaceBases);
	virtual FRotator GetDefaultOwnerPawnAimRotation(USkeletalMeshComponent * MeshComp);
	void EvaluateAdditiveBoneRotation(USkeletalMeshComponent* MeshComp, UAnimInstance* Animation, struct FCompactPose& outPose);

	void FillInTraces(const float MaxTime, const float MinTime, float CurrentEvaluationTime, float PreviousEvaluationTime, USkeletalMeshComponent* MeshComp, UAnimSequenceBase * Animation, const FTransform& PreviousMeshWorldTransform);
	void DrawCapsuleTrace(USkeletalMeshComponent* MeshComp, const FVector StartLocation, const FVector EndLocation, const FVector StrikeDirection);
};
