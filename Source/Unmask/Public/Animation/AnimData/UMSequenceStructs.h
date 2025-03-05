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
	FRotatorRange(const FRotator& StartIn, const FRotator& EndIn) : Min(StartIn), Max(EndIn){}
	
	UPROPERTY(Blueprintable, BlueprintReadWrite)
	FRotator Min {FRotator(0.f)};
	UPROPERTY(Blueprintable, BlueprintReadWrite) 
	FRotator Max {FRotator(0.f)};
};

// Represents a single state of a "ctrl" in the control rig for the given joint
USTRUCT(Blueprintable, BlueprintType)
struct FUMJointControl
{
	GENERATED_BODY()
public:
	UPROPERTY(Blueprintable, BlueprintReadWrite)
	FName CtrlName;
	UPROPERTY(Blueprintable, BlueprintReadWrite)
	FRotator CtrlRotator{FRotator(0, 0, 0)};
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
	FUMJoint(FName InName, bool IsFk = false) : Name(InName), CtrlName(InName.ToString() + (IsFk ?  "_fk" : "") + "_ctrl") {};
	FUMJoint(FName InName, FName InCtrl, FRotatorRange InRangeLimits, FUMJointTimeline InTimeline) :
		Name(InName), CtrlName(InCtrl), RangeLimits(InRangeLimits), Timeline(InTimeline)
	{};
public:
	UPROPERTY(Blueprintable, BlueprintReadWrite)
	FName Name;
	UPROPERTY(Blueprintable, BlueprintReadWrite)
	FName CtrlName;
	UPROPERTY(Blueprintable, BlueprintReadWrite)
	FRotatorRange RangeLimits {FRotatorRange()};
	UPROPERTY(Blueprintable, BlueprintReadWrite)
	FUMJointTimeline Timeline; // Collections of keyframes that have been set for the joint
public:
	FString ToString();
};


class UUMJointGroup;
