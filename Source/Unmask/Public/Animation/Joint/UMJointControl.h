// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UMJointAxis.h"
#include "UMJointStructs.h"
#include "UObject/Object.h"
#include "UMJointControl.generated.h"

class UCameraComponent;
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
	TArray<AUMJointAxis*> AxesRefs = {nullptr, nullptr, nullptr}; //size 3: 0 = x, 1 = y, 2 = z
	UPROPERTY(BlueprintReadOnly)
	TArray<FVector> AxesBases = {{}, {}, {}}; //mirrors AxesRefs
	UPROPERTY(BlueprintReadOnly)
	AUMJointAxis* CameraAxis = nullptr;
	UPROPERTY(BlueprintReadOnly)
	UUMJoint* Parent = nullptr;
	UPROPERTY()
	FName Name = "**";
	UPROPERTY()
	FName BoneName = "*";
protected:
	UPROPERTY()
	USkeletalMeshComponent* SkeletalMeshComponent {nullptr};
	FUMControlTransform State;
	FUMControlRange Range;
	bool bIsValid = false;
	int Mode = 0;

public:
	// If this Control has any valid axes, it's valid
	bool IsActive() const
	{
		auto IsAxisValid = [](AUMJointAxis* Axis) -> bool{ return Axis != nullptr;};
		return AxesRefs.FilterByPredicate(IsAxisValid).Num() > 0;
	}

	FVector GetLocation();
	void Setup(FName CtrlName, const FRotatorRange& InRange, /*For axis*/ FName BoneName, USkeletalMeshComponent* SkeletalMeshComponent);
	UFUNCTION(BlueprintCallable, BlueprintGetter)
	FUMControlTransform GetState() { return State; }
	void SetState(const FUMControlTransform &InState);

	UFUNCTION(BlueprintCallable)
	void Update();
	
	UFUNCTION(BlueprintCallable)
	UPARAM(DisplayName="Success") bool
	Bind(UCameraComponent *Camera);

	UFUNCTION(BlueprintCallable)
	void DeselectAxis(EUMJointAxisType AxisType) const;

	UFUNCTION(BlueprintCallable)
	void Unbind();

	UFUNCTION(BlueprintCallable)
	void SetVisibility(bool bCameraVisibility, bool bAxesVisibility);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	AUMJointAxis* GetAxis(const EUMJointAxisType Axis) const
	{
		if (Axis != X && Axis != Y && Axis != Z) { return nullptr; }
		return AxesRefs[Axis-1];
	}
protected:
};
