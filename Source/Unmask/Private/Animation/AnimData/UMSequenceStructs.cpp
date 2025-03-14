#include "Animation/AnimData/UMSequenceStructs.h"
#include "Animation/UMAnimationProducer.h"


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

