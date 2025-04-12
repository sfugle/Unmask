// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "UMJointAxis.generated.h"

class UCameraComponent;
class UUMJointControl;

UENUM(BlueprintType)
enum EUMJointAxisType
{
	Null = 0,
	X,
	Y,
	Z,
	Camera
};

UCLASS(BlueprintType)
class UNMASK_API AUMJointAxis : public AStaticMeshActor
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EUMJointAxisType> AxisType;
	UPROPERTY(BlueprintReadWrite)
	UCameraComponent* CameraRef;
protected:
	UPROPERTY()
	UUMJointControl* Parent;
	UPROPERTY()
	float Min = 0.f;
	UPROPERTY()
	float State = 0.f;
	UPROPERTY()
	float Max = 0.f;
	UPROPERTY()
	bool bUsingMinMax = true;
	FTransform InitialTransform;
public:
	// Sets default values for this actor's properties
	AUMJointAxis();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Setup(UUMJointControl* InParent, EUMJointAxisType AxisTypeIn,
	           float InMin, float InMax, bool bIgnoreMinMax = false);
	UFUNCTION(BlueprintCallable)
	void SetVisibility(bool Vis);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetPosition() { return GetStaticMeshComponent()->GetComponentLocation(); }
	UFUNCTION(BlueprintCallable)
	void ModifyState(float delta)
	{
		if(bUsingMinMax)
		{
			State = FMath::Clamp(State + delta, Min, Max);
		} else
		{
			State += delta;
		}
		
	}
};
