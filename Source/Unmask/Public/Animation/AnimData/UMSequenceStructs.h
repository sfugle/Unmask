#pragma once

#include "UMSequenceStructs.generated.h"

class UUMSequenceHelper;

/*
 * JointGroup and JointControl are both located
 * in the UMJointClasses file
 */

USTRUCT(Blueprintable, BlueprintType)
struct FRotatorRange
{
	GENERATED_BODY()
	
	FRotatorRange(){}
	FRotatorRange(const FRotator& StartIn, const FRotator& EndIn) : Min(StartIn.GetNormalized()), Max(EndIn.GetNormalized()){ ComputeRange(); }
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FRotator Min {FRotator::ZeroRotator};
	UPROPERTY(EditAnywhere, BlueprintReadOnly) 
	FRotator Max {FRotator::ZeroRotator};
	
protected:
	bool bDirty = true;
	FQuat MidQuat;
	float MaxAngularDistance = 0.f;
public:

	void ComputeRange()
	{
		auto MinQuat = Min.GetNormalized().Quaternion();
		auto MaxQuat = Max.GetNormalized().Quaternion();
		MidQuat = FQuat::Slerp(MinQuat, MaxQuat, 0.5);
		MaxAngularDistance = FQuat::ErrorAutoNormalize(MinQuat, MaxQuat);
		bDirty = false;
	}
	
	bool Within(const FRotator& Rotator) const
	{
		check(!bDirty)
		return FQuat::ErrorAutoNormalize(Rotator.Quaternion(), MidQuat) <= MaxAngularDistance;
	}
};

// Represents a "ctrl" in the control rig and imparts upon it limits
USTRUCT(Blueprintable, BlueprintType)
struct FUMControlRange
{
	GENERATED_BODY()
public:
	FUMControlRange() {}
	FUMControlRange(FName InName) : Name(InName) {}
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name = "**";
	// Current Rotations are stored via index
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotatorRange RotatorRange {FRotatorRange(FRotator(0, 0, 0), FRotator(0, 0, 0))};
};

USTRUCT(Blueprintable, BlueprintType)
struct FUMControlTransform
{
	GENERATED_BODY()
public:
	FUMControlTransform() {}
	FUMControlTransform(FName InName) : Name(InName) {}
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name = "**";
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform Transform = FTransform::Identity;
};

// Represents a joint's state at a given point in time
USTRUCT(Blueprintable, BlueprintType)
struct FUMJointKey
{
	GENERATED_BODY()

public:

	constexpr bool operator<(const FUMJointKey& Other) const
	{
		return Time < Other.Time;
	}
	UPROPERTY(Blueprintable, BlueprintReadWrite)
	float Time = 0.f;
	
	UPROPERTY(Blueprintable, BlueprintReadWrite)
	FTransform Transform = {};

};


// To let the JointKey array be serialized
USTRUCT(Blueprintable, BlueprintType)
struct FUMJointTimeline
{
	GENERATED_BODY()
public:
	FUMJointTimeline() {}
	FUMJointTimeline(const TArray<FUMJointKey>& JointTimelineIn) : Timeline(JointTimelineIn)
	{
		this->Duration = Timeline[Timeline.Num() - 1].Time;
	}
	UPROPERTY(Blueprintable, BlueprintReadWrite)
	TArray<FUMJointKey> Timeline;
	UPROPERTY(BlueprintReadWrite)
	float StartTime = 0;
	UPROPERTY(BlueprintReadWrite)
	float Duration = 0;
public:
	FString ToString();
};

// Represents a bone within the pose-able skeleton
// Holds its own timeline of keys, its own name, and the limit that joint can be rotated within
USTRUCT(Blueprintable, BlueprintType)
struct FUMJoint
{
	GENERATED_BODY()
public:
	FUMJoint() : Name("*") {};
	FUMJoint(FName InName, bool IsFk = false) : Name(InName), Control(FName(InName.ToString() + (IsFk ?  "_fk" : "") + "_ctrl")) {};
	FUMJoint(FName InName, FName InCtrl, FRotatorRange InRangeLimits, FUMJointTimeline InTimeline) :
		Name(InName), Control(InCtrl), RangeLimits(InRangeLimits), Timeline(InTimeline)
	{};
public:
	UPROPERTY(Blueprintable, BlueprintReadWrite)
	FName Name;
	UPROPERTY(Blueprintable, BlueprintReadWrite)
	FUMControlRange Control;
	int Depth = -1;
	UPROPERTY(Blueprintable, BlueprintReadWrite)
	FRotatorRange RangeLimits {FRotatorRange()};
	UPROPERTY(Blueprintable, BlueprintReadWrite)
	FUMJointTimeline Timeline; // Collections of keyframes that have been set for the joint
public:
	FString ToString();
};

class UUMJointGroup;

UCLASS()
class UUMSequenceHelper :  public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintPure, Category = "Animation|Sequencer", meta=(BlueprintThreadSafe))
	static FUMJointKey MakeKeyframe(float Time, const FTransform& Transform);

	UFUNCTION(BlueprintCallable, Category = "Animation|Joint|Group")
	static const void PrintJointGroup(UUMJointGroup *Group);
	
	UFUNCTION(Category = "Animation|Joint|Group", meta=(BlueprintThreadSafe))
	static void AddGroups(UUMJointGroup *JointGroup, const TArray<UUMJointGroup*> GroupsIn);
	
	UFUNCTION(Category = "Animation|Joint|Group", meta=(BlueprintThreadSafe))
	static void AddGroup(UUMJointGroup *JointGroup, UUMJointGroup *Group);
	
	UFUNCTION(Category = "Animation|Joint|Group", meta=(BlueprintThreadSafe))
	static void AddJoints(UUMJointGroup *JointGroup, TArray<FUMJoint>& JointsIn);

	UFUNCTION(Category = "Animation|Joint|Group", meta=(BlueprintThreadSafe))
	static void AddJoint(UUMJointGroup *JointGroup, const FUMJoint& Joint);

	UFUNCTION(BlueprintPure, Category = "Animation", meta=(BlueprintThreadSafe))
	static UAnimSequence* BuildSequence(UUMJointGroup *JointGroup, USkeletalMesh* SkeletalMesh);
};

