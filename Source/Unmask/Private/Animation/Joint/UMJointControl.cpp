// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Joint/UMJointControl.h"

#include "Animation/Joint/UMJointAxis.h"

void UUMJointControl::Setup(FName CtrlName, const FRotatorRange& InRange, /*For axis*/ FName SocketName, USkeletalMeshComponent* SkeletalMeshComponent)
{
	Name = CtrlName;
	State = FUMControlTransform(CtrlName);
	Range = FUMControlRange(CtrlName);
	Range.RotatorRange = InRange;
	Range.RotatorRange.ComputeRange();
	int nameIncr = 0;
	for(int i = 0; i < 3; i++)
	{
		float Min = Range.RotatorRange.Min.GetComponentForAxis(static_cast<EAxis::Type>(i+1));
		float Max = Range.RotatorRange.Max.GetComponentForAxis(static_cast<EAxis::Type>(i+1));

		if(FMath::IsNearlyEqual(Min, 0.f) && FMath::IsNearlyEqual(Max, 0.f))
		{
			continue;
		}
		
		FString AxisName = CtrlName.ToString() + "_Axis_" + FString::FromInt(i) + "_" + FString::FromInt(nameIncr++);
		
		FVector BoneLocation = SkeletalMeshComponent->GetBoneLocation(SocketName, EBoneSpaces::Type::ComponentSpace);
		FVector Location = BoneLocation + Range.InitialTransforms[i].GetTranslation();
		FQuat BoneRotation = SkeletalMeshComponent->GetBoneTransform(SocketName, RTS_Component).GetRotation();
		FVector AxisDirection =
			i == 0 ?
				FVector::ForwardVector:
				i == 1 ?
					FVector::RightVector :
					FVector::UpVector;
		FRotator Rotation = FRotator(Range.InitialTransforms[i].GetRotation() * AxisDirection.ToOrientationQuat());
		
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnInfo.Name = MakeUniqueObjectName(this, AUMJointAxis::StaticClass(), FName(AxisName));
		
		AxesRefs[i] = SkeletalMeshComponent->GetWorld()->SpawnActor<AUMJointAxis>(Location, Rotation, SpawnInfo);
		AxesRefs[i]->SetActorScale3D(Range.InitialTransforms[i].GetScale3D());
		
		FAttachmentTransformRules AttachmentTransformRules = {EAttachmentRule::KeepRelative, true};
		AxesRefs[i]->AttachToComponent(SkeletalMeshComponent, AttachmentTransformRules, SocketName);
		AxesRefs[i]->Setup(this, Min, Max);
	}
}

void UUMJointControl::SetState(const FUMControlTransform& InState)
{
	State = InState;
}
