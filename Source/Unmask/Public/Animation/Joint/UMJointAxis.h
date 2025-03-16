// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UMJointControl.h"
#include "Engine/StaticMeshActor.h"
#include "UMJointAxis.generated.h"

UCLASS(BlueprintType)
class UNMASK_API AUMJointAxis : public AStaticMeshActor
{
	GENERATED_BODY()
protected:
	UPROPERTY()
	UUMJointControl* Parent;
	UPROPERTY()
	float Min = 0;
	UPROPERTY()
	float Max = 0;
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
	void Setup(UUMJointControl* InParent,
	           float InMin, float InMax);
	void SetVisbility(bool Vis);
};
