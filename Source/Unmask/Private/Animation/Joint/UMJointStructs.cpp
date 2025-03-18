#include "Animation/Joint/UMJointStructs.h"
#include "Animation/Joint/UMJointGroup.h"

FString UUMJoint::ToString()
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
	JointGroup->Groups.Add(Group);
	Group->Parent = JointGroup;
}

void UUMSequenceHelper::AddJoints(UUMJointGroup* JointGroup, TArray<UUMJoint*>& JointsIn)
{
	JointGroup->Joints.Reserve(JointGroup->Groups.Num() + JointsIn.Num());
	for (auto& Joint : JointsIn) { AddJoint(JointGroup, Joint); }
}

void UUMSequenceHelper::AddJoint(UUMJointGroup* JointGroup, UUMJoint* Joint)
{
	JointGroup->Joints.Add(Joint);
}
