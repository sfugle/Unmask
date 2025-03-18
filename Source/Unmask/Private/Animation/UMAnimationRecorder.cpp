// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/UMAnimationRecorder.h"
#include "Animation/Skeleton.h"
#include "Animation/AnimData/UMSequenceStructs.h"
#include "Animation/Joint/UMJoint.h"
#include "Animation/Joint/UMJointControl.h"
#include "Animation/Joint/UMJointGroup.h"
#include "Animation/Joint/UMJointStructs.h"

class UUMSequenceHelper;
class UUMAnimationRecorder;

UUMAnimationRecorder::UUMAnimationRecorder()
{
	this->SkeletalMeshComponent = nullptr;
	RootGroup = NewObject<UUMJointGroup>();
	RootGroup->Name = "Root";
	RootGroup->Depth = 0;
	SelectedGroup = RootGroup;
	bGenerated = false;
}

UUMAnimationRecorder* UUMAnimationRecorder::GetAnimationRecorder()
{
	return NewObject<UUMAnimationRecorder>();
}


void UUMAnimationRecorder::InitAnimationRecorder(USkeletalMeshComponent* InSkeletalMeshComponent, TMap<FName, FRotatorRange> InControlRanges)
{
	this->SkeletalMeshComponent = InSkeletalMeshComponent;
	this->DefaultControlRanges = InControlRanges;
	
	auto MatInsts = SkeletalMeshComponent->GetMaterials();
	
	for(auto MatInst : MatInsts)
	{
		DynMatInsts.Add(UMaterialInstanceDynamic::Create(MatInst, this));
	}
	for(int i = 0; i < DynMatInsts.Num(); i++)
	{
		SkeletalMeshComponent->SetMaterial(i, DynMatInsts[i]);
	}

	FUMJointsAggregate BlankAggregate = FUMJointsAggregate(SkeletalMeshComponent, {});
	FUMJointsAggregate &NewJoints = BlankAggregate;
	
	this->RootGroup->Name = FName("body");
	RootGroup->AddBones({"root", "pelvis", "spine_01", "spine_02", "spine_03", "spine_04", "spine_05", "clavicle_l", "neck_01", "neck_02", "head",
		"clavicle_r", });
	this->AllGroups.Add(this->RootGroup->Name, this->RootGroup);
	UUMJoint* LeftShoulder = NewObject<UUMJoint>()->Init(FName("upperarm_l"), true);
	UUMJoint* RightShoulder = NewObject<UUMJoint>()->Init(FName("upperarm_r"), true);
	UUMJoint* Pelvis = NewObject<UUMJoint>()->Init(FName("pelvis"));
	UUMJoint* Spine01 = NewObject<UUMJoint>()->Init(FName("spine_01"));
	UUMJoint* Spine02 = NewObject<UUMJoint>()->Init(FName("spine_02"));
	UUMJoint* Spine03 = NewObject<UUMJoint>()->Init(FName("spine_03"));
	UUMJoint* LeftClavicle = NewObject<UUMJoint>()->Init(FName("clavicle_l"));
	UUMJoint* RightClavicle = NewObject<UUMJoint>()->Init(FName("clavicle_r"));
	UUMJoint* Neck = NewObject<UUMJoint>()->Init(FName("neck_01"));
	UUMJoint* HeadJoint = NewObject<UUMJoint>()->Init(FName("head"));
	NewJoints = BlankAggregate;
	NewJoints = CreateJoints(RootGroup->Name,
		{Neck, HeadJoint, LeftShoulder, RightShoulder, Pelvis, Spine01, Spine02, Spine03, LeftClavicle, RightClavicle}
		);
	RootGroup->AddJoints(NewJoints);
	
	UUMJointGroup *LeftLeg = NewObject<UUMJointGroup>();
	LeftLeg->Name = FName("leg_l");
	AllGroups.Add(LeftLeg->Name, LeftLeg);
	LeftLeg->AddBones({"thigh_l", "calf_l", "calf_twist_01_l", "calf_twist_02_l", "thigh_twist_01_l",
		"thigh_twist_02_l", "foot_l", "ball_l"});
	UUMJoint* LeftThigh = NewObject<UUMJoint>()->Init(FName("thigh_l"), true);
	UUMJoint* LeftCalf = NewObject<UUMJoint>()->Init(FName("calf_l"), true);
	NewJoints = BlankAggregate;
	NewJoints = CreateJoints(LeftLeg->Name, {LeftThigh, LeftCalf});
	LeftLeg->AddJoints(NewJoints);
	RootGroup->AddGroup(LeftLeg);

	UUMJointGroup *RightLeg = NewObject<UUMJointGroup>();
	RightLeg->Name =  FName("leg_r");
	AllGroups.Add(RightLeg->Name, RightLeg);
	RightLeg->AddBones({"thigh_r","calf_r", "calf_twist_01_r", "calf_twist_02_r", "thigh_twist_01_r", "thigh_twist_02_r", "foot_r", "ball_r"});
	UUMJoint* RightThigh = NewObject<UUMJoint>()->Init(FName("thigh_r"), true);
	UUMJoint* RightCalf = NewObject<UUMJoint>()->Init(FName("calf_r"), true);
	NewJoints = BlankAggregate;
	NewJoints = CreateJoints(RightLeg->Name, {RightThigh, RightCalf});
	RightLeg->AddJoints(NewJoints);
	RootGroup->AddGroup(RightLeg);
	
	UUMJointGroup *LeftArm  = NewObject<UUMJointGroup>();
	LeftArm->Name = FName("arm_l");
	AllGroups.Add(LeftArm->Name, LeftArm);
	LeftArm->AddBones({"thumb_03_l", "lowerarm_l", "lowerarm_twist_01_l", "lowerarm_twist_02_l", "upperarm_l"
	});
	UUMJoint* LowerLeftArm = NewObject<UUMJoint>()->Init(FName("lowerarm_l"));
	UUMJoint* LeftWrist = NewObject<UUMJoint>()->Init(FName("hand_l"), true);
	NewJoints = BlankAggregate;
	NewJoints = CreateJoints(LeftArm->Name, {LowerLeftArm, LeftWrist});
	LeftArm->AddJoints(NewJoints);
	RootGroup->AddGroup(LeftArm);
	
	UUMJointGroup *RightArm = NewObject<UUMJointGroup>();
	RightArm->Name = FName("arm_r");
	AllGroups.Add(RightArm->Name, RightArm);
	RightArm->AddBones({"upperarm_r", "lowerarm_r", "lowerarm_twist_01_r", "lowerarm_twist_02_r"});
	UUMJoint* 	LowerRightArm = NewObject<UUMJoint>()->Init(FName("lowerarm_r"), true);
	UUMJoint* 	RightWrist = NewObject<UUMJoint>()->Init(FName("hand_r"), true);
	NewJoints = BlankAggregate;
	NewJoints = CreateJoints(RightArm->Name,{LowerRightArm, RightWrist});
	RightArm->AddJoints(NewJoints);
	RootGroup->AddGroup(RightArm);

	UUMJointGroup *LeftHand = NewObject<UUMJointGroup>();
	LeftHand->Name = FName("hand_l");
	AllGroups.Add(LeftHand->Name, LeftHand);
	LeftHand->AddBones({"hand_l", "index_metacarpal_l", "index_01_l", "index_02_l", "index_03_l",
		"middle_metacarpal_l", "middle_01_l", "middle_02_l", "middle_03_l", "pinky_metacarpal_l", "pinky_01_l",
		"pinky_02_l", "pinky_03_l", "ring_metacarpal_l", "ring_01_l", "ring_02_l", "ring_03_l", "thumb_01_l",
		"thumb_02_l",});
		//LeftThumb(FName("thumb_02_l")),
	UUMJoint* LeftIndex = NewObject<UUMJoint>()->Init(FName("index_metacarpal_l"));
	UUMJoint* LeftMiddle = NewObject<UUMJoint>()->Init(FName("middle_metacarpal_l"));
	NewJoints = BlankAggregate;
	NewJoints = CreateJoints(LeftHand->Name,{LeftIndex, LeftMiddle});
	LeftHand->AddJoints(NewJoints);
	LeftArm->AddGroup(LeftHand);
	
	UUMJointGroup* RightHand = NewObject<UUMJointGroup>();
	RightHand->Name = FName("hand_r");
	AllGroups.Add(RightHand->Name, RightHand);
	RightHand->AddBones({"thumb_03_r", "hand_r", "index_metacarpal_r", "index_01_r", "index_02_r", "index_03_r",
		"middle_metacarpal_r", "middle_01_r", "middle_02_r", "middle_03_r", "pinky_metacarpal_r", "pinky_01_r",
		"pinky_02_r", "pinky_03_r", "ring_metacarpal_r", "ring_01_r", "ring_02_r", "ring_03_r", "thumb_01_r",
		"thumb_02_r"});
		//RightThumb(FName("thumb_01_r")),
	UUMJoint* RightIndex = NewObject<UUMJoint>()->Init(FName("index_metacarpal_r"));
	UUMJoint* RightMiddle = NewObject<UUMJoint>()->Init(FName("middle_metacarpal_r"));
	NewJoints = BlankAggregate;
	NewJoints = CreateJoints(RightHand->Name, {RightIndex, RightMiddle});
	RightHand->AddJoints(NewJoints);
	RightArm->AddGroup(RightHand);

	for (auto Pair : AllGroups)
	{
		VisibleGroups.Add(Pair.Key);
	}
	GeneratePoseData();
}

FString UUMAnimationRecorder::AllGroupsToString()
{
	auto StringBuilder = new TStringBuilder<1024 * sizeof(FString)>();
	StringBuilder->Append(TEXT("["));
	for (auto Tuple : AllGroups)
	{
		auto& Name = Tuple.Key;
		auto& Group = Tuple.Value;
		StringBuilder->Append(Name.ToString() + TEXT(": \n\tJoints: [") );
		for (int i = 0; i < Group->Joints.Num(); i++)
		{
			auto& Joint = Group->Joints[i];
			StringBuilder->Append(*Joint->ToString());
			if (i < Group->Joints.Num() - 1)
			{
				StringBuilder->Append(TEXT(", "));
			}
		}
		StringBuilder->Append(TEXT("]\n\tGroups: ["));
		for (int i = 0; i < Group->Groups.Num(); i++)
		{
			auto& ChildGroup = Group->Groups[i];
			if(ChildGroup->Name.IsValid())
			{
				StringBuilder->Append(ChildGroup->Name.ToString());
			} else
			{
				StringBuilder->Append("*");
			}
			
			if (i < Group->Groups.Num() - 1)
			{
				StringBuilder->Append(TEXT(", "));
			}
		}
		StringBuilder->Append(TEXT("]\n"));
	}
	return StringBuilder->ToString();
}

void UUMAnimationRecorder::PrintAllGroups()
{
	FString out = AllGroupsToString();
	UE_LOG(LogCore, Display, TEXT("\n%s"), *out);
	if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::White,  FString::Printf(TEXT("%s"), *out));
}

void UUMAnimationRecorder::LoadTimelines(TMap<FName, FUMJointTimeline> Timelines)
{
	for (TTuple<FName, FUMJointTimeline> Timeline : Timelines)
	{
		for (TTuple<FName, UUMJointGroup*> Tuple : this->AllGroups)
		{
			bool Exit = false;
			for (int i = 0; i < Tuple.Value->Joints.Num(); i++)
			{
				if (Tuple.Value->Joints[i]->Name == Timeline.Key)
				{
					Exit = true;
					(*this->AllGroups.Find(Tuple.Key))->Joints[i]->Timeline = Timeline.Value;
					break;
				}
			}
			if (Exit) break;
		}
	}
}

TMap<FName, int> UUMAnimationRecorder::GetControlIndexMap()
{
	return IndexMap;
}

TArray<FRotatorRange> UUMAnimationRecorder::GetControlRanges()
{
	return RotatorRanges;
}

int UUMAnimationRecorder::GetControlIndex(const FName Name)
{
	if(!bGenerated)
	{
		GeneratePoseData();
	}
	return IndexMap.FindChecked(Name);
}

FRotatorRange UUMAnimationRecorder::GetControlRange(FName Name)
{
	if(!bGenerated)
	{
		GeneratePoseData();
	}
	const int Index = IndexMap.FindChecked(Name);
	return RotatorRanges[Index];
}

void UUMAnimationRecorder::UpdateControlValue(TArray<FUMControlTransform>& PoseDataRef, int Index, FUMControlTransform NewControlTransform)
{
	if(!bGenerated)
	{
		GeneratePoseData();
	}
	ensure(Index < PoseDataRef.Num() && Index < RotatorRanges.Num());
	check(PoseDataRef[Index].Name.IsEqual(NewControlTransform.Name));
	
	if(RotatorRanges[Index].Within(NewControlTransform.Transform.Rotator()))
	{
		PoseDataRef[Index] = NewControlTransform;
	}
	for (auto& JointAggregate : AllJointAggregates)
	{
		JointAggregate.Value.Update();
	}
}

void UUMAnimationRecorder::GeneratePoseData()
{
	TArray<TPair<FName, UUMJoint*>> AllJointPairs;
	TArray<FUMControlTransform> JointControls;
	for (auto JointGroup : AllJointAggregates)
	{
		FName &GroupName = JointGroup.Key;
		for (auto Joint : JointGroup.Value.JointArray)
		{
			AllJointPairs.Add(TPair<FName, UUMJoint*>(GroupName, Joint));
		}
	}
	AllJointPairs.Sort(
		[](const TPair<FName, UUMJoint*>& Pair1, const TPair<FName, UUMJoint*>& Pair2) -> bool
			{return Pair1.Value->Depth < Pair2.Value->Depth;}
	);
	IndexMap.Reserve(AllJointPairs.Num());
	for (int i = 0; i < AllJointPairs.Num(); i++)
	{
		IndexMap.Add(AllJointPairs[i].Key, i);
	}
	RotatorRanges.Empty();
	for (auto& JointPair : AllJointPairs)
	{
		UUMJoint* &Joint = JointPair.Value;
		FName CtrlName = Joint->ControlName;
		
		FRotatorRange RangeVal = FRotatorRange(FRotator::ZeroRotator, FRotator::ZeroRotator);
		FRotatorRange* RangePtr = DefaultControlRanges.Find(CtrlName);
		if(RangePtr)
		{
			RangeVal = *RangePtr;
			RangeVal.ComputeRange();
		}
		Joint->Control->Setup(CtrlName, RangeVal, FName(Joint->Name.ToString()+FString("Socket")), SkeletalMeshComponent);
		JointControls.Add(Joint->Control->GetState());
		RotatorRanges.Add(RangeVal);
	}
	
	bGenerated = true;
	InitialPoseData = JointControls;
}


UUMJointGroup* UUMAnimationRecorder::GetGroupByBone(FName BoneName)
{
	for (auto Tuple : AllGroups)
	{
		auto &Group = Tuple.Value;
		if(Group->Bones.Contains(BoneName))
		{
			UE_LOG(LogCore, Warning, TEXT("%s"), *Group->Name.ToString());
			return Group;
		}
	}
	UE_LOG(LogScript, Warning, TEXT("Failed to find group with Bone %s"), *BoneName.ToString());
	return nullptr;
}

bool UUMAnimationRecorder::SelectGroupByName(FName Name, bool bForce = false)
{

	auto Group = AllGroups.Find(Name);
	if(Group)
	{
		SelectGroup(*Group, bForce);
		return true;
	} else {
		UE_LOG(LogScript, Warning, TEXT("[AnimationRecorder] Invalid Group name \"%s\" for SelectGroupByName"), *Name.ToString())
		return false;
	}

}

void UUMAnimationRecorder::SelectGroup(UUMJointGroup* Group, bool bForce=false)
{
	TMap<FName, EUMAnimEditor_MatSlot> MatSlotMap = {
		{"body", TORSO},
		{"leg_l", LEG_L},
		{"leg_r", LEG_R},
		{"arm_l", ARM_L},
		{"arm_r", ARM_R},
		{"hand_l", HAND_L},
		{"hand_r", HAND_R}
	};

	if(!VisibleGroups.Contains(Group->Name) and !bForce)
	{
		FString fs;
		for(auto Name : VisibleGroups)
		{
			fs += Name.ToString() + ",";
		}
		UE_LOG(LogCore, Warning, TEXT("VisibleGroups: %s"), *fs);
		return;
	}

	if(Group == RootGroup)
	{
		for(int i = 0; i < DynMatInsts.Num(); i++){
			DynMatInsts[i]->SetScalarParameterValue("bVisible", 1.0);
			DynMatInsts[i]->SetScalarParameterValue("bHighlighted", 0.0);
		}
		VisibleGroups.Empty();
		for (auto Pair : AllGroups)
		{
			VisibleGroups.Add(Pair.Key);
		}
		SelectedGroup = RootGroup;
		return;
	}
	AdjacentGroups.Empty();
	TArray<int> MatSlots;
	for(auto Child : Group->Groups)
	{
		AdjacentGroups.Add(Child->Name);
	}
	if(Group->Parent)
	{
		AdjacentGroups.Add(Group->Parent->Name);
	} else
	{
		UE_LOG(LogScript, Error, TEXT("Invalid parent for %s"), *Group->Name.ToString())
	}
	VisibleGroups.Empty();
	VisibleGroups = AdjacentGroups;
	
	for (FName Name : AdjacentGroups)
	{
		UE_LOG(LogCore, Display, TEXT("Name: %s"), *Name.ToString())
		MatSlots.Add(MatSlotMap.FindChecked(Name));
		DynMatInsts[MatSlots.Last()]->SetScalarParameterValue("bHighlighted", 1.0);
	}

	MatSlots.Add(MatSlotMap.FindChecked(Group->Name));
	DynMatInsts[MatSlots.Last()]->SetScalarParameterValue("bHighlighted", 0.0);
	VisibleGroups.Add(Group->Name);
	
	for (int i = 0; i < DynMatInsts.Num(); i++)
	{
		if(MatSlots.Contains(i))
		{
			DynMatInsts[i]->SetScalarParameterValue("bVisible", 1.0);
			continue;
		}
		DynMatInsts[i]->SetScalarParameterValue("bVisible", 0.0);
		DynMatInsts[i]->SetScalarParameterValue("bHighlighted", 0.0);
	}
	GeneratePoseData();
	SelectedGroup = Group;
}

FUMJointsAggregate& UUMAnimationRecorder::CreateJoints(FName GroupName, const TArray<UUMJoint*>& InJoints)
{
	for (UUMJoint* Joint : InJoints)
	{
		AllJoints.Add(Joint->Name, Joint);
		UE_LOG(LogScript, Warning, TEXT("Joint: %s"), *Joint->Name.ToString())
	}
	return AllJointAggregates.Add(GroupName, FUMJointsAggregate(SkeletalMeshComponent, InJoints));
}

TMap<FName, FVector> UUMAnimationRecorder::GetJointPositions()
{
	TMap<FName, FVector> Return;
	for (auto Joint : SelectedGroup->Joints)
	{
		Return.Add(Joint->Name, SkeletalMeshComponent->GetBoneTransform(Joint->Name, RTS_World).GetTranslation());
	}
	return Return;
}

TMap<FName, FVector> UUMAnimationRecorder::GetGroupPositions()
{
	TMap<FName, FVector> Return;
	for (auto Group : SelectedGroup->Groups)
	{
		auto JointAggregate = AllJointAggregates.Find(Group->Name);
		if(JointAggregate) Return.Add(Group->Name, JointAggregate->JointPosSum / JointAggregate->JointArray.Num());
	}
	if(SelectedGroup->Parent)
	{
		FName ParentName = SelectedGroup->Parent->Name;
		auto JointAggregate = AllJointAggregates.Find(ParentName);
		if(JointAggregate) Return.Add(ParentName, JointAggregate->JointPosSum / JointAggregate->JointArray.Num());
	}
	return Return;
}

