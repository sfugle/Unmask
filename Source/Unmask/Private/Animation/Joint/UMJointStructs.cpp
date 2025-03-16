#include "Animation/Joint/UMJointStructs.h"
#include "Animation/Joint/UMJointGroup.h"
#include "Animation/Joint/UMJointControl.h"

FUMJoint::FUMJoint() : Name("**"), Control(NewObject<UUMJointControl>()){}

FUMJoint::FUMJoint(FName InName, bool IsFk)  : Name(InName),
	ControlName(FName(InName.ToString() + (IsFk ?  "_fk" : "") + "_ctrl")), Control(NewObject<UUMJointControl>()){}

FUMJoint::FUMJoint(FName InName, FName InCtrl, FRotatorRange InRangeLimits, const FUMJointTimeline& InTimeline)
: Name(InName), ControlName(InCtrl), Control(NewObject<UUMJointControl>()), Timeline(InTimeline){}


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