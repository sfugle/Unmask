// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Joint/UMJointAxis.h"


// Sets default values
AUMJointAxis::AUMJointAxis() : Parent(nullptr)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	this->SetMobility(EComponentMobility::Type::Movable);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshClass(TEXT("/Script/Engine.StaticMesh'/Game/UM_MVP/ring.ring'"));
	if(StaticMeshClass.Succeeded())
	{
		this->GetStaticMeshComponent()->SetStaticMesh(StaticMeshClass.Object);
	} else {
		UE_LOG(LogScript, Error, TEXT("[UMJointAxis] ctor: Missing mesh for Joint Axes"))
	}
	this->GetStaticMeshComponent()->SetRenderCustomDepth(true);
	this->GetStaticMeshComponent()->SetVisibility(false);
}

// Called when the game starts or when spawned
void AUMJointAxis::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUMJointAxis::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AUMJointAxis::Setup(UUMJointControl* InParent, float InMin, float InMax)
{
	Parent = InParent;
	
}

void AUMJointAxis::SetVisbility(bool Vis)
{
	this->GetStaticMeshComponent()->SetVisibility(Vis);
}



