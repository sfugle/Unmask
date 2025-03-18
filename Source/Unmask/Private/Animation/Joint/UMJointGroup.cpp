#include "Animation/Joint/UMJointGroup.h"
#include "Animation/UMAnimationProducer.h"

// Store the depth of the joint in its group, then add all the joints to pose data sorted by their depth. 
// This ensures that the parent is never applied after the child.
//Add bounds as a struct (of froters) at blueprint level for easy modification.

void UUMJointGroup::AddGroup(UUMJointGroup* Group)
{
	Groups.Add(Group);
	Group->Parent = this;
	Group->Depth = this->Depth + 1;
	for (auto Joint : Group->Joints)
	{
		Joint->Depth = this->Depth + 1;
	}
}


FString UUMJointGroup::ToString()
{
	auto StringBuilder = new TStringBuilder<1024 * sizeof(FString)>();
	TArray<TPair<UUMJointGroup*, int>> JointGroupStack;
	JointGroupStack.Push(TPair<UUMJointGroup*, int>(this, 0));
	while(!JointGroupStack.IsEmpty())
	{
		const TPair<UUMJointGroup*, int> JointGroup_Depth = JointGroupStack.Pop();
		const auto JointGroup = JointGroup_Depth.Key;
		const int TabDepth = JointGroup_Depth.Value;
		
		FString TabString = "";
		for(int i = 0; i < TabDepth; i++)
		{
			TabString += "    ";
		}
		StringBuilder->Append(TabString + JointGroup->Name.ToString() + TEXT("\n"));
		TabString += "  ";
		StringBuilder->Append(TabString + TEXT("Joints: "));
		for (int i = 0; i < JointGroup->Joints.Num(); i++)
		{
			auto& Joint = JointGroup->Joints[i];
			StringBuilder->Append(Joint->ToString());
			if (i <JointGroup->Joints.Num() - 1)
			{
				StringBuilder->Append(TEXT(", "));
			}
		}
		StringBuilder->Append(TabString + TEXT("Bones: "));
		for (int i = 0; i < JointGroup->Bones.Num(); i++)
		{
			auto& Bone = JointGroup->Bones.Array()[i];
			StringBuilder->Append(*Bone.ToString());
			if (i <JointGroup->Bones.Num() - 1)
			{
				StringBuilder->Append(TEXT(", "));
			}
		}
		StringBuilder->Append(TEXT("\n"));
		StringBuilder->Append(TabString + TEXT("Groups:\n "));
		for (auto Group : JointGroup->Groups)
		{
			JointGroupStack.Push(TPair<UUMJointGroup*, int>(Group, TabDepth+1));
		}
	}
	
	return StringBuilder->ToString();
}

bool UUMJointGroup::ContainsBone(FName BoneName, bool bIncludeChildren)
{
	if(this->Bones.Contains(BoneName))
	{
		return true;
	}
	if(!bIncludeChildren)
	{
		return false;
	}
	for (UUMJointGroup* JG : this->Groups)
	{
		if (JG->ContainsBone(BoneName, true)) return true;
	}
	return false;
}

FUMJointKey UUMSequenceHelper::MakeKeyframe(float Time, const FTransform &Transform)
{
	return {Time, Transform};
}

const void UUMSequenceHelper::PrintJointGroup(UUMJointGroup* Group)
{
	FString out = Group->ToString();
	UE_LOG(LogCore, Display, TEXT("\n%s"), *out);
	if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::White,  FString::Printf(TEXT("%s"), *out));
}

UAnimSequence* UUMSequenceHelper::BuildSequence(UUMJointGroup* JointGroup, USkeletalMesh* SkeletalMesh)
{
	TMap<FName, FUMJointTimeline> Timelines;
	TQueue<UUMJointGroup*> Queue;
	Queue.Enqueue(JointGroup);
	while (!Queue.IsEmpty())
	{
		UUMJointGroup* Group;
		Queue.Dequeue(Group);
		for (UUMJointGroup* SubGroup : Group->Groups)
		{
			Queue.Enqueue(SubGroup);
		}
		for (UUMJoint* Joint : Group->Joints)
		{
			float Scale = Joint->Timeline.Duration / Joint->Timeline.Timeline[Joint->Timeline.Timeline.Num() - 1].Time;
			FUMJointTimeline NewTimeline;
			for (FUMJointKey Key : Joint->Timeline.Timeline)
			{
				FUMJointKey NewKey;
				NewKey.Time = (Key.Time * Scale) + Joint->Timeline.StartTime;
				NewKey.Transform = Key.Transform;
				NewTimeline.Timeline.Add(NewKey);
			}
			Timelines.Add(Joint->Name, NewTimeline);
		}
	}
	return UUMAnimationProducer::CreateSequence(Timelines, SkeletalMesh);
}