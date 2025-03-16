
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Joint/UMJointStructs.h"
#include "Joint/UMJoint.h"
#include "Templates/Tuple.h"
#include "UObject/Object.h"
#include "UMAnimationRecorder.generated.h"

/**
 * 
 */



template<typename... Types>  struct TTuple;

USTRUCT()
struct FUMParentChildPair
{
	GENERATED_BODY()
	int32 ParentIndex, ChildIndex;
};

UENUM()
enum EUMAnimEditor_MatSlot
{
	TORSO = 0,
	LEG_L, 
	LEG_R,
	ARM_L,
	ARM_R,
	HAND_L,
	HAND_R,
};

USTRUCT(BlueprintType)
struct FUMJointsAggregate
{
	GENERATED_BODY()
public:
	FVector JointPosSum;
	UPROPERTY()
	TArray<UUMJoint*> JointArray;
	UPROPERTY()
	const USkeletalMeshComponent* SMC;
public:
	FUMJointsAggregate() : SMC(nullptr)
	{
		UE_LOG(LogScript, Error, TEXT("[JointAggregate] Made with default constructor"))
	};
	FUMJointsAggregate(const USkeletalMeshComponent *SkeletalMeshComponent, const TArray<UUMJoint*>& Joints)
	{
		SMC = SkeletalMeshComponent;
		JointArray = Joints;
	}

	void Update()
	{
		JointPosSum = FVector::ZeroVector;
		for (auto Joint : JointArray)
		{
			JointPosSum += SMC->GetBoneTransform(Joint->Name, RTS_World).GetTranslation();
		}
	}
};

UCLASS(BlueprintType)
class UNMASK_API UDA_UMControlRanges : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TMap<FName, FRotatorRange> InitialControlRanges;
};

UCLASS(Blueprintable, BlueprintType)
class UNMASK_API UUMAnimationRecorder : public UObject
{
	GENERATED_BODY()
protected:
	UPROPERTY()
	USkeletalMeshComponent* SkeletalMeshComponent;
	
	UPROPERTY()
	UUMJointGroup* RootGroup;
	
	UPROPERTY()
	TMap<FName, UUMJointGroup*> AllGroups;

	UPROPERTY()
	TMap<FName, UUMJoint*> AllJoints;

	UPROPERTY()
	TMap<FName, FUMJointsAggregate> AllJointAggregates;
	
	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> DynMatInsts;
	
	UPROPERTY()
	TArray<FName> VisibleGroups;

	UPROPERTY()
	TArray<FName> AdjacentGroups;


// Pose Data
public:
	UPROPERTY(BlueprintReadOnly)
	UUMJointGroup* SelectedGroup;
	
	UPROPERTY(BlueprintReadOnly)
	TArray<FUMControlTransform> InitialPoseData;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TMap<FName, FRotatorRange> DefaultControlRanges;

	// array, sorted so that the parents come before their children. ensures no out of order ctrl application
	UPROPERTY()
	TArray<FRotatorRange> RotatorRanges;

	UPROPERTY()
	TMap<FName, int> IndexMap; //maps a control name to its place in the above array
	
	bool bGenerated;
	

public:
	UUMAnimationRecorder();
	
	UFUNCTION(BlueprintCallable)
	static UUMAnimationRecorder* GetAnimationRecorder();
	
	UFUNCTION(BlueprintCallable)
	void InitAnimationRecorder(USkeletalMeshComponent* InSkeletalMeshComponent, TMap<FName, FRotatorRange> InControlRanges);
	
	UFUNCTION(BlueprintCallable)
	void GeneratePoseData();

	UFUNCTION(BlueprintCallable)
	void LoadTimelines(TMap<FName, FUMJointTimeline> Timelines);
	
	FString AllGroupsToString();
	
	UFUNCTION(BlueprintCallable)
	void PrintAllGroups();
	

	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TMap<FName, UUMJointGroup*> GetAllGroups() { return AllGroups; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TMap<FName, UUMJoint*> GetAllJoints() { return AllJoints; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UUMJointGroup* GetGroupByBone(FName Bone);

	UFUNCTION(BlueprintCallable)
	bool SelectGroupByName(FName Name, bool bForce);
	
	UFUNCTION(BlueprintCallable)
	void SelectGroup(UUMJointGroup* Group, bool bForce);

	UFUNCTION(BlueprintCallable)
	void UpdateControlValue(UPARAM(ref) TArray<FUMControlTransform>& PoseDataRef, int Index, FUMControlTransform NewControlTransform);
	
	
	UFUNCTION(BluePrintCallable, BlueprintGetter)
	UUMJointGroup* GetRootGroup() { return this->RootGroup; }
	
	UFUNCTION(BluePrintCallable, BlueprintGetter)
	TMap<FName, UUMJointGroup*> GetGroups() { return this->AllGroups; }

	UFUNCTION(BlueprintCallable, BlueprintGetter)
	TMap<FName, int> GetControlIndexMap();

	UFUNCTION(BlueprintCallable, BlueprintGetter)
	TArray<FRotatorRange> GetControlRanges();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetControlIndex(FName Name);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FRotatorRange GetControlRange(FName Name);


	UFUNCTION(BlueprintCallable, BlueprintPure)	
	TMap<FName, FVector> GetJointPositions();
	
	UFUNCTION(BlueprintCallable, BlueprintPure)	
	TMap<FName, FVector> GetGroupPositions();

protected:
	FUMJointsAggregate& CreateJoints(FName GroupName, const TArray<UUMJoint*>& InJoints);
};