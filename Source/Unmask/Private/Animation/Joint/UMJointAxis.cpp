// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Joint/UMJointAxis.h"
#include "Camera/CameraComponent.h"

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
	SetVisibility(false);
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
	if(AxisType == Camera && IsValid(CameraRef))
	{
		// Ring Mesh is facing on its side, so we need the right/up vector instead;
		this->SetActorRotation(CameraRef->GetUpVector().Rotation());
	}
}

void AUMJointAxis::Setup(UUMJointControl* InParent, const EUMJointAxisType InAxisType, float InMin, float InMax, bool bIgnoreMinMax)
{
	Parent = InParent;
	this->AxisType = InAxisType;
	this->GetStaticMeshComponent()->SetCustomDepthStencilValue(static_cast<int>(InAxisType));
	if(bIgnoreMinMax)
	{
		bUsingMinMax = false;
		return;
	}
	Min = InMax;
	Max = InMax;
	
}

void AUMJointAxis::Select()
{
	bIsSelected = true;
	GetStaticMeshComponent()->SetCustomDepthStencilValue(AxisType + 10);
}

void AUMJointAxis::Deselect()
{
	bIsSelected = false;
	GetStaticMeshComponent()->SetCustomDepthStencilValue(AxisType);
}

void AUMJointAxis::SetVisibility(bool Vis)
{
	this->GetStaticMeshComponent()->SetVisibility(Vis, true);
	Deselect();
	if(Vis)
	{
		this->GetStaticMeshComponent()->SetCollisionProfileName("BlockAll", true);
	}
	else {
		this->GetStaticMeshComponent()->SetCollisionProfileName("NoCollision", false);
	}
	
}



