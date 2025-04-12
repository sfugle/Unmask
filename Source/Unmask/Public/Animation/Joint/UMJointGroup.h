#pragma once

#include "Animation/UMAnimationRecorder.h"
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
	FName TrackName;
	UPROPERTY(Blueprintable, BlueprintReadWrite)
	UUMJointGroup* Parent {nullptr};
	int Depth = -1;
	UPROPERTY(Blueprintable, BlueprintReadWrite)
	TArray<UUMJointGroup*> Groups;
	UPROPERTY(Blueprintable, BlueprintReadWrite)
	TArray<UUMJoint*> Joints;
	TSet<FName> Bones;
public:
	UUMJointGroup() : Name("*"), TrackName("Track Name") {}
	void Setup(const FName InName, const FName InTrackName)
	{
		Name = InName;
		TrackName = InTrackName;
	}
	
	void AddGroups(TArray<UUMJointGroup*>& GroupsIn) { for (const auto& Group : GroupsIn) { AddGroup(Group); }}
	void AddGroup(UUMJointGroup* Group);
	void AddJoints(FUMJointsAggregate &JointsAggregateIn) { for (const auto& Joint : JointsAggregateIn.JointArray) { AddJoint(Joint); } }
	void AddJoints(TArray<UUMJoint*>& JointsIn) { for (const auto& Joint : JointsIn) { AddJoint(Joint); } }
	void AddJoint(UUMJoint* Joint)  { Joints.Add(Joint); Joint->Parent = this; }
	void AddBones(TArray<FName> InBones) { for (const auto& Bone : InBones) { AddBone(Bone); }}
	void AddBone(const FName InBone) { Bones.Add(InBone); }
	
	FString ToString();
	bool ContainsBone(FName BoneName, bool bIncludeChildren);

private:
	
	friend UUMSequenceHelper;
};
