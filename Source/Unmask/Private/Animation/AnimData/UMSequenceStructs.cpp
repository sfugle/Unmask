#include "Animation/AnimData/UMSequenceStructs.h"
#include "Animation/UMAnimationProducer.h"
#include "Animation/AnimData/UMJointGroup.h"


FString FUMJointTimeline::ToString()
{
	FString s = FString("");
	for(int i = 0; i < this->Timeline.Num(); i++)
	{
		auto Keyframe = this->Timeline[i];
		s += FString::SanitizeFloat(Keyframe.Time) + ": " + Keyframe.Transform.ToString();
		if (i < this->Timeline.Num() - 1)
		{
			s += ", ";
		}
	}
	return "Timeline: [" + s + "]";
}

FString FUMJoint::ToString()
{
	return "Joint " + this->Name.ToString() + ": Depth " + FString::FromInt(this->Depth) + ", Ctrl " +
		this->ControlName.ToString() + ", Timeline" + this->Timeline.ToString();
}

void UUMSequenceHelper::AddGroups(UUMJointGroup *JointGroup, const TArray<UUMJointGroup*> GroupsIn)
{
	JointGroup->Groups.Reserve(JointGroup->Groups.Num() + GroupsIn.Num());
	for (auto& Group : GroupsIn) { AddGroup(JointGroup, Group); }
}

void UUMSequenceHelper::AddGroup(UUMJointGroup* JointGroup, UUMJointGroup* Group)
{
	// No reserve tricks because they screw up amortized constant time
	JointGroup->Groups.Add(Group);
	Group->Parent = JointGroup;
}

void UUMSequenceHelper::AddJoints(UUMJointGroup* JointGroup, TArray<FUMJoint>& JointsIn)
{
	JointGroup->Joints.Reserve(JointGroup->Groups.Num() + JointsIn.Num());
	for (auto& Joint : JointsIn) { AddJoint(JointGroup, Joint); }
}

void UUMSequenceHelper::AddJoint(UUMJointGroup* JointGroup, const FUMJoint& Joint)
{
	JointGroup->Joints.Add(Joint);
}
