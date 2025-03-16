// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UMJointStructs.h"
#include "UObject/Object.h"
#include "../AnimData/UMSequenceStructs.h"
#include "UMJoint.generated.h"

class UUMJointControl;
class UUMJointGroup;
// Represents a bone within the pose-able skeleton
// Holds its own timeline of keys, its own name, and the limit that joint can be rotated within
UCLASS(BlueprintType)
class UUMJoint : public UObject
{
	GENERATED_BODY()
public:
	//UUMJoint();
	UUMJoint* Init(FName InName, bool IsFk = false);
	//UUMJoint(FName InName, FName InCtrl, FRotatorRange InRangeLimits, const FUMJointTimeline& InTimeline);
public:
	UPROPERTY()
	UUMJointGroup* Parent = nullptr;
	UPROPERTY(BlueprintReadWrite)
	FName Name;
	UPROPERTY(BlueprintReadOnly)
	UUMJointControl* Control;
	UPROPERTY(BlueprintReadWrite)
	FName ControlName;
	int Depth = -1;
	UPROPERTY(Blueprintable, BlueprintReadWrite)
	FUMJointTimeline Timeline; // Collections of keyframes that have been set for the joint
public:
	UFUNCTION(BlueprintCallable)
	UUMJointControl* GetControl() const { return Control; };
	FString ToString();
};
