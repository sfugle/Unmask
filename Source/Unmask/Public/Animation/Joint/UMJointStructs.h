#pragma once
//#include "UMJointControl.h"
#include "Animation/AnimData/UMSequenceStructs.h"
#include "UMJointStructs.generated.h"

class UUMJointControl;
class UUMJointGroup;

USTRUCT(Blueprintable, BlueprintType)
struct FRotatorRange
{
	GENERATED_BODY()
	
	FRotatorRange() {}
	FRotatorRange(const FRotator& StartIn, const FRotator& EndIn) : Min(StartIn.GetNormalized()), Max(EndIn.GetNormalized()){ ComputeRange(); }
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FRotator Min {FRotator::ZeroRotator};
	UPROPERTY(EditAnywhere, BlueprintReadOnly) 
	FRotator Max {FRotator::ZeroRotator};
protected:
	bool bDirty = true;
	
public:
	//bool bDirty = true;
	FQuat MidQuat;
	float MaxAngularDistance = 0.f;
public:
	void SetRange(const FRotator& StartIn, const FRotator& EndIn)
	{
		Min = StartIn.GetNormalized();
		Max = EndIn.GetNormalized();
		ComputeRange();
	}
	void ComputeRange()
	{
		auto MinQuat = Min.GetNormalized().Quaternion();
		auto MaxQuat = Max.GetNormalized().Quaternion();
		MidQuat = FQuat::SlerpFullPath_NotNormalized(MinQuat, MaxQuat, 0.5).GetNormalized();
		// UE_LOG(LogScript, Warning, TEXT("Min: %s, Mid: %s, Max: %s, Angle: %f"),
		// 	*MinQuat.Rotator().ToCompactString(), *MidQuat.Rotator().ToCompactString(),
		// 	*MaxQuat.Rotator().ToCompactString(), MaxAngularDistance);
		MaxAngularDistance = FQuat::Error(MinQuat, MidQuat);
		bDirty = false;
	}
	
	bool Within(const FRotator& Rotator) const
	{
		check(!bDirty)
		return FQuat::Error(Rotator.Quaternion(), MidQuat) <= MaxAngularDistance;
	}
};


// Represents a bone within the pose-able skeleton
// Holds its own timeline of keys, its own name, and the limit that joint can be rotated within
USTRUCT(Blueprintable, BlueprintType)
struct FUMJoint
{
	GENERATED_BODY()
public:
	FUMJoint();
	FUMJoint(FName InName, bool IsFk = false);
	FUMJoint(FName InName, FName InCtrl, FRotatorRange InRangeLimits, const FUMJointTimeline& InTimeline);
public:
	UPROPERTY()
	UUMJointGroup* Parent = nullptr;
	UPROPERTY(Blueprintable, BlueprintReadWrite)
	FName Name;
	UPROPERTY(Blueprintable, BlueprintReadWrite)
	FName ControlName;
	UPROPERTY(Blueprintable, BlueprintReadOnly)
	UUMJointControl* Control;
	int Depth = -1;
	UPROPERTY(Blueprintable, BlueprintReadWrite)
	FUMJointTimeline Timeline; // Collections of keyframes that have been set for the joint
public:
	FString ToString();
};

// Specifically used to pass FName data along with a 
USTRUCT(Blueprintable, BlueprintType)
struct FUMControlRange
{
	GENERATED_BODY()
public:
	FUMControlRange() {}
	FUMControlRange(FName InName) : Name(InName){}
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName Name = "**";
	// Current Rotations are stored in FUMControlTransform via index

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FRotatorRange RotatorRange {FRotatorRange(FRotator(0, 0, 0), FRotator(0, 0, 0))};

	// size 3 array, x = 0, y = 1, z = 2
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FTransform> InitialTransforms = TArray<FTransform>{FTransform::Identity, FTransform::Identity, FTransform::Identity};
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
