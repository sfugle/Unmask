#pragma once

#include "Animation/Joint/UMJointStructs.h"
#include "UMJointGroup.generated.h"

class UUMSequenceHelper;


// A container for multiple Joints, which can also hold other JointGroups
// Also holds a flattened list of all the Timelines represented within
// Note: underlying Timeline maps being edited does not propagate upward yet
UCLASS(Blueprintable, BlueprintType)
class UUMJointGroup : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Blueprintable, BlueprintReadWrite)
	FName Name;
	UPROPERTY(Blueprintable, BlueprintReadWrite)
	UUMJointGroup* Parent {nullptr};
	int Depth = -1;
	UPROPERTY(Blueprintable, BlueprintReadWrite)
	TArray<UUMJointGroup*> Groups;
	TArray<FUMJoint> Joints;
	TSet<FName> Bones;
public:
	UUMJointGroup() : Name("*") {}
	UUMJointGroup(FName InName) : Name(InName) {}
	
	void AddGroups(TArray<UUMJointGroup*>& GroupsIn) { for (const auto& Group : GroupsIn) { AddGroup(Group); }}
	void AddGroup(UUMJointGroup* Group);
	void AddJoints(TArray<FUMJoint> JointsIn) { for (auto& Joint : JointsIn) { AddJoint(Joint); } }
	void AddJoint(FUMJoint& Joint)  { Joints.Add(Joint); }
	void AddBones(TArray<FName> InBones) { for (auto& Bone : InBones) { AddBone(Bone); }}
	void AddBone(FName InBone) { Bones.Add(InBone); }
	
	FString ToString();
	bool ContainsBone(FName BoneName, bool bIncludeChildren);

private:
	
	friend UUMSequenceHelper;
};
