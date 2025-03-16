// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UMJointStructs.h"
#include "UObject/Object.h"
#include "UMJointControl.generated.h"

class AUMJointAxis;
/**
 * Acts as the back face of the axes that the players directly manipulate to change the joint values.
 */
UCLASS(BlueprintType)
class UNMASK_API UUMJointControl : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
	TArray<bool> AxesEnabled = {false, false, false}; //size 3: 0 = x, 1 = y, 2 = z
	UPROPERTY(BlueprintReadOnly)
	TArray<AUMJointAxis*> AxesRefs = {{}, {}, {}}; //size 3: 0 = x, 1 = y, 2 = z
protected:
	UPROPERTY()
	FName Name = "**";
	FUMControlTransform State;
	FUMControlRange Range;

public:
	
	void Setup(FName CtrlName, const FRotatorRange& InRange, /*For axis*/ FName SocketName, USkeletalMeshComponent* SkeletalMeshComponent);
	UFUNCTION(BlueprintCallable, BlueprintGetter)
	FUMControlTransform GetState() { return State; }
	void SetState(const FUMControlTransform &InState);
};
