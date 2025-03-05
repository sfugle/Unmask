#pragma once
#include "UMSequenceStructs.h"
#include "UMJointClasses.generated.h"


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
	UPROPERTY(Blueprintable, BlueprintReadWrite)
	TArray<UUMJointGroup*> Groups;
	TArray<FUMJoint> Joints;
	TSet<FName> Bones;
public:
	UUMJointGroup() : Name("*") {}
	UUMJointGroup(FName InName) : Name(InName) {}
	
	void AddGroups(TArray<UUMJointGroup*>& GroupsIn) { for (const auto& Group : GroupsIn) { AddGroup(Group); }}
	void AddGroup(UUMJointGroup* Group) { Groups.Add(Group); Group->Parent = this;}
	void AddJoints(TArray<FUMJoint> JointsIn) { for (auto& Joint : JointsIn) { AddJoint(Joint); } }
	void AddJoint(const FUMJoint& Joint) { Joints.Add(Joint); }
	void AddBones(TArray<FName> InBones) { for (auto& Bone : InBones) { AddBone(Bone); }}
	void AddBone(FName InBone) { Bones.Add(InBone); }
	
	FString ToString();
	bool ContainsBone(FName BoneName, bool bIncludeChildren);

private:
	
	friend UUMSequenceHelper;
};

UCLASS()
class UUMSequenceHelper :  public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintPure, Category = "Animation|Sequencer", meta=(BlueprintThreadSafe))
	static FUMJointKey MakeKeyframe(float Time, const FTransform& Transform);

	UFUNCTION(BlueprintCallable, Category = "Animation|Joint|Group")
	static const void PrintJointGroup(UUMJointGroup *Group);
	
	UFUNCTION(Category = "Animation|Joint|Group", meta=(BlueprintThreadSafe))
	static void AddGroups(UUMJointGroup *JointGroup, const TArray<UUMJointGroup*> GroupsIn)
	{
		JointGroup->Groups.Reserve(JointGroup->Groups.Num() + GroupsIn.Num());
		for (auto& Group : GroupsIn) { AddGroup(JointGroup, Group); }
	}
	UFUNCTION(Category = "Animation|Joint|Group", meta=(BlueprintThreadSafe))
	static void AddGroup(UUMJointGroup *JointGroup, UUMJointGroup *Group)
	{
		// No reserve tricks because they screw up amortized constant time
		JointGroup->Groups.Add(Group);
		Group->Parent = JointGroup;
	}
	UFUNCTION(Category = "Animation|Joint|Group", meta=(BlueprintThreadSafe))
	static void AddJoints(UUMJointGroup *JointGroup, TArray<FUMJoint>& JointsIn)
	{
		JointGroup->Joints.Reserve(JointGroup->Groups.Num() + JointsIn.Num());
		for (auto& Joint : JointsIn) { AddJoint(JointGroup, Joint); }
	}
	UFUNCTION(Category = "Animation|Joint|Group", meta=(BlueprintThreadSafe))
	static void AddJoint(UUMJointGroup *JointGroup, const FUMJoint& Joint)
	{
		JointGroup->Joints.Add(Joint);
	}
	UFUNCTION(BlueprintPure, Category = "Animation", meta=(BlueprintThreadSafe))
	static UAnimSequence* BuildSequence(UUMJointGroup *JointGroup, USkeletalMesh* SkeletalMesh);
};