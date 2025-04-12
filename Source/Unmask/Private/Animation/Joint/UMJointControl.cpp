// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Joint/UMJointControl.h"

#include "Animation/Joint/UMJointAxis.h"
#include "Camera/CameraComponent.h"

FVector UUMJointControl::GetLocation()
{
	return SkeletalMeshComponent->GetComponentTransform().TransformPosition(SkeletalMeshComponent->GetBoneLocation(BoneName, EBoneSpaces::Type::ComponentSpace));
}

void UUMJointControl::Setup(FName CtrlName, const FRotatorRange& InRange, /*For axis*/ FName InBoneName, USkeletalMeshComponent* InSkeletalMeshComponent)
{
	Name = CtrlName;
	BoneName = InBoneName;
	FName SocketName = FName(BoneName.ToString() + FString("Socket"));
	State = FUMControlTransform(CtrlName);
	Range = FUMControlRange(CtrlName);
	Range.RotatorRange = InRange;
	Range.RotatorRange.ComputeRange();
	FVector MaxScale = FVector::ZeroVector;
	SkeletalMeshComponent = InSkeletalMeshComponent;

		//SkeletalMeshComponent->GetBoneTransform(BoneName, RTS_Component).Rotator().Quaternion();
	for(int i = 1; i <= 3; i++)
	{
		int ID = i - 1;
		EAxis::Type AxisType = StaticCast<EAxis::Type>(i);
		EUMJointAxisType UMAxisType = StaticCast<EUMJointAxisType>(i);
		float Min = Range.RotatorRange.Min.GetComponentForAxis(AxisType);
		float Max = Range.RotatorRange.Max.GetComponentForAxis(AxisType);

		bool UseMinMax = true;
		if(FMath::IsNearlyEqual(Min, 0.f) && FMath::IsNearlyEqual(Max, 0.f))
		{
			UseMinMax = false;
			//continue;
		}
		bIsValid = true;
		FString AxisName = CtrlName.ToString() + "_Axis_" + FString::FromInt(i);
		FQuat StartingQuat = FQuat::Identity;
		if(AxisType == EAxis::Type::Y)
		{
			StartingQuat *= FRotator(0, 90, 0).Quaternion();
		} else if(AxisType == EAxis::Type::Z)
		{
			StartingQuat *= FRotator(90, 0, 0).Quaternion();
		}
		// AxisVector.Normalize();
		// AxesBa	ses[ID] = StartingRotation.Vector();
		
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnInfo.Name = MakeUniqueObjectName(this, AUMJointAxis::StaticClass(), FName(AxisName));
		auto World = SkeletalMeshComponent->GetWorld();
		ensure(World);
		FVector BoneLocation = SkeletalMeshComponent->GetBoneLocation(SocketName, EBoneSpaces::Type::ComponentSpace);
		FVector Location = SkeletalMeshComponent->GetBoneLocation(BoneName, EBoneSpaces::Type::WorldSpace); //SkeletalMeshComponent->GetComponentLocation()
		AxesRefs[ID] = World->SpawnActor<AUMJointAxis>(BoneLocation, StartingQuat.Rotator(), SpawnInfo);
		ensure(AxesRefs[ID]);
		AxesRefs[ID]->SetActorScale3D(Range.InitialTransforms[ID].GetScale3D());
		MaxScale = FVector::Max(MaxScale, Range.InitialTransforms[ID].GetScale3D());
		
		FAttachmentTransformRules AttachmentTransformRules = {EAttachmentRule::KeepRelative, true};
		AxesRefs[ID]->AttachToComponent(SkeletalMeshComponent, AttachmentTransformRules, SocketName);
		AxesRefs[ID]->Setup(this, UMAxisType, Min, Max, UseMinMax);
	}
	// if the joint is active, create camera axis
	if(bIsValid) 
	{
		FVector BoneLocation = SkeletalMeshComponent->GetBoneLocation(BoneName, EBoneSpaces::Type::ComponentSpace);
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnInfo.Name = MakeUniqueObjectName(this, AUMJointAxis::StaticClass(), FName(CtrlName.ToString() + "_Camera"));
		
		CameraAxis = SkeletalMeshComponent->GetWorld()->SpawnActor<AUMJointAxis>(BoneLocation, FRotator::ZeroRotator, SpawnInfo);
		CameraAxis->SetVisibility(false);
		CameraAxis->SetActorScale3D(MaxScale);
		
		// FAttachmentTransformRules AttachmentTransformRules = {EAttachmentRule::KeepRelative, true};
		// CameraAxis->AttachToComponent(SkeletalMeshComponent, AttachmentTransformRules, SocketName);
		CameraAxis->Setup(this, EUMJointAxisType::Camera, 0, 0);
	}
}

	void UUMJointControl::SetState(const FUMControlTransform& InState)
{
	State = InState;
}

void UUMJointControl::Update()
{
	return;
	for(auto Axis : AxesRefs)
	{
		if(Axis)
			Axis->SetActorLocation(GetLocation());
	}
}

bool UUMJointControl::Bind(UCameraComponent *Camera)
{
	if(!bIsValid) { return false; }
	Mode++;
	switch (Mode) {
		case 1:
			SetVisibility(false, true);
			return true;
		// case 2:
		// 	CameraAxis->CameraRef = Camera;
		// 	SetVisibility(true, false);
		// 	return true;
		default:
			SetVisibility(false, false);
			return false;
	}
}

void UUMJointControl::Unbind()
{
	if(!bIsValid) { return; }
	Mode = 0;
	SetVisibility(false, false);
}

void UUMJointControl::SetVisibility(bool bCameraVisibility, bool bAxesVisibility)
{
	CameraAxis->SetVisibility(bCameraVisibility);
	if(bAxesVisibility)
	{
		Update();
	}
	for (auto Axis : AxesRefs)
	{
		if(Axis)
		{
			Axis->SetVisibility(bAxesVisibility);
		} else
		{
			UE_LOG(LogScript, Error, TEXT("Axis ref invalid for set vis"))
		}
	}
}
