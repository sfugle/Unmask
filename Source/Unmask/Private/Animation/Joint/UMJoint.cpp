// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Joint/UMJoint.h"
#include "Animation/Joint/UMJointControl.h"

// UUMJoint::UUMJoint() : Name("**"), Control(NewObject<UUMJointControl>()){}

UUMJoint* UUMJoint::Init(FName InName, bool IsFk)
{
	Name = InName;
	ControlName = FName(InName.ToString() + (IsFk ?  "_fk" : "") + "_ctrl");
	Control = NewObject<UUMJointControl>();
	Control->Parent = this;
	return this;
}

// UUMJoint::UUMJoint(FName InName, FName InCtrl, FRotatorRange InRangeLimits, const FUMJointTimeline& InTimeline)
// : Name(InName), ControlName(InCtrl), Control(NewObject<UUMJointControl>()), Timeline(InTimeline){}
