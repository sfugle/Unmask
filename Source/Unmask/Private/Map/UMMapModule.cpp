// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/UMMapModule.h"
#include "Map/UMPathLocation.h"

UUMMapModule* UUMMapModule::InitTestingMapModule()
{
	UUMMapModule* out = NewObject<UUMMapModule>();
	FVector L1 = FVector(1360, 1880, -50);
	FVector L2 = FVector(1860, 2380, -50);
	FVector L3 = FVector(860, 2380, -50);
	FVector L4 = FVector(1360, 2880, -50);
	UUMPathLocation* PL4 = NewObject<UUMPathLocation>();
	PL4->Location = L4;
	UUMPathLocation* PL3 = NewObject<UUMPathLocation>();
	PL3->Location = L3;
	PL3->NextLocations.Add(PL4);
	UUMPathLocation* PL2 = NewObject<UUMPathLocation>();
	PL2->Location = L2;
	PL2->NextLocations.Add(PL4);
	UUMPathLocation* PL1 = NewObject<UUMPathLocation>();
	PL1->Location = L1;
	PL1->NextLocations.Add(PL2);
	PL1->NextLocations.Add(PL3);
	out->SouthToNorthStart = PL1;
	return out;
}

UUMMapModule* UUMMapModule::InitModule(int ID)
{
	UUMMapModule* Out = NewObject<UUMMapModule>();
	switch (ID)
	{
	case 0:
		{
			int InterchangeDistanceFromCenter = 80;
			int EntranceDistanceFromCenter = 525;
			int ExitDistanceFromCenter = 614;
			FVector NorthEntrance = FVector(EntranceDistanceFromCenter, -InterchangeDistanceFromCenter, 0);
			FVector NorthExit = FVector(ExitDistanceFromCenter, InterchangeDistanceFromCenter, 0);
			FVector EastEntrance = FVector(InterchangeDistanceFromCenter, EntranceDistanceFromCenter, 0);
			FVector EastExit = FVector(-InterchangeDistanceFromCenter, ExitDistanceFromCenter, 0);
			FVector SouthEntrance = FVector(-EntranceDistanceFromCenter, InterchangeDistanceFromCenter, 0);
			FVector SouthExit = FVector(-ExitDistanceFromCenter, -InterchangeDistanceFromCenter, 0);
			FVector WestEntrance = FVector(-InterchangeDistanceFromCenter, -EntranceDistanceFromCenter, 0);
			FVector WestExit = FVector(InterchangeDistanceFromCenter, -ExitDistanceFromCenter, 0);
			FVector NorthEastCenter = FVector(InterchangeDistanceFromCenter, InterchangeDistanceFromCenter, 0);
			FVector SouthEastCenter = FVector(-InterchangeDistanceFromCenter, InterchangeDistanceFromCenter, 0);
			FVector SouthWestCenter = FVector(-InterchangeDistanceFromCenter, -InterchangeDistanceFromCenter, 0);
			FVector NorthWestCenter = FVector(InterchangeDistanceFromCenter, -InterchangeDistanceFromCenter, 0);
			TArray<FVector> NorthToNorthPath;
			NorthToNorthPath.Add(NorthEntrance);
			NorthToNorthPath.Add(NorthExit);
			Out->NorthToNorthStart = BuildLinearPath(NorthToNorthPath);
			TArray<FVector> NorthToEastPath;
			NorthToEastPath.Add(NorthEntrance);
			NorthToEastPath.Add(SouthWestCenter);
			NorthToEastPath.Add(EastExit);
			Out->NorthToEastStart = BuildLinearPath(NorthToEastPath);
			TArray<FVector> NorthToSouthPath;
			NorthToSouthPath.Add(NorthEntrance);
			NorthToSouthPath.Add(SouthExit);
			Out->NorthToSouthStart = BuildLinearPath(NorthToSouthPath);
			TArray<FVector> NorthToWestPath;
			NorthToWestPath.Add(NorthEntrance);
			NorthToWestPath.Add(NorthWestCenter);
			NorthToWestPath.Add(WestExit);
			Out->NorthToWestStart = BuildLinearPath(NorthToWestPath);
			TArray<FVector> EastToSouthPath;
			EastToSouthPath.Add(EastEntrance);
			EastToSouthPath.Add(NorthWestCenter);
			EastToSouthPath.Add(SouthExit);
			Out->EastToSouthStart = BuildLinearPath(EastToSouthPath);
			TArray<FVector> EastToEastPath;
			EastToEastPath.Add(EastEntrance);
			EastToEastPath.Add(EastExit);
			Out->EastToEastStart = BuildLinearPath(EastToEastPath);
			TArray<FVector> EastToWestPath;
			EastToWestPath.Add(EastEntrance);
			EastToWestPath.Add(WestExit);
			Out->EastToWestStart = BuildLinearPath(EastToWestPath);
			TArray<FVector> EastToNorthPath;
			EastToNorthPath.Add(EastEntrance);
			EastToNorthPath.Add(NorthEastCenter);
			EastToNorthPath.Add(NorthExit);
			Out->EastToNorthStart = BuildLinearPath(EastToNorthPath);
			TArray<FVector> SouthToWestPath;
			SouthToWestPath.Add(SouthEntrance);
			SouthToWestPath.Add(NorthEastCenter);
			SouthToWestPath.Add(WestExit);
			Out->SouthToWestStart = BuildLinearPath(SouthToWestPath);
			TArray<FVector> SouthToSouthPath;
			SouthToSouthPath.Add(SouthEntrance);
			SouthToSouthPath.Add(SouthExit);
			Out->SouthToSouthStart = BuildLinearPath(SouthToSouthPath);
			TArray<FVector> SouthToNorthPath;
			SouthToNorthPath.Add(SouthEntrance);
			SouthToNorthPath.Add(NorthExit);
			Out->SouthToNorthStart = BuildLinearPath(SouthToNorthPath);
			TArray<FVector> SouthToEastPath;
			SouthToEastPath.Add(SouthEntrance);
			SouthToEastPath.Add(SouthEastCenter);
			SouthToEastPath.Add(EastExit);
			Out->SouthToEastStart = BuildLinearPath(SouthToEastPath);
			TArray<FVector> WestToNorthPath;
			WestToNorthPath.Add(WestEntrance);
			WestToNorthPath.Add(SouthEastCenter);
			WestToNorthPath.Add(NorthExit);
			Out->WestToNorthStart = BuildLinearPath(WestToNorthPath);
			TArray<FVector> WestToWestPath;
			WestToWestPath.Add(WestEntrance);
			WestToWestPath.Add(WestExit);
			Out->WestToWestStart = BuildLinearPath(WestToWestPath);
			TArray<FVector> WestToEastPath;
			WestToEastPath.Add(WestEntrance);
			WestToEastPath.Add(EastExit);
			Out->WestToEastStart = BuildLinearPath(WestToEastPath);
			TArray<FVector> WestToSouthPath;
			WestToSouthPath.Add(WestEntrance);
			WestToSouthPath.Add(SouthWestCenter);
			WestToSouthPath.Add(SouthExit);
			Out->WestToSouthStart = BuildLinearPath(WestToSouthPath);
			break;
		}
	default:
		UE_LOG(LogScript, Warning, TEXT("No tag given to this Map Module."));
	}
	return Out;
}

double UUMMapModule::DistanceToClosestPath(FVector PlayerLocation)
{
	double Out = -1;
	
	TQueue<UUMPathLocation*> LocationQueue;
	LocationQueue.Enqueue(this->GetNorthToEastStart());
	LocationQueue.Enqueue(this->GetNorthToSouthStart());
	LocationQueue.Enqueue(this->GetNorthToWestStart());
	LocationQueue.Enqueue(this->GetNorthToNorthStart());
	LocationQueue.Enqueue(this->GetSouthToWestStart());
	LocationQueue.Enqueue(this->GetSouthToNorthStart());
	LocationQueue.Enqueue(this->GetSouthToEastStart());
	LocationQueue.Enqueue(this->GetSouthToSouthStart());
	LocationQueue.Enqueue(this->GetEastToSouthStart());
	LocationQueue.Enqueue(this->GetEastToWestStart());
	LocationQueue.Enqueue(this->GetEastToNorthStart());
	LocationQueue.Enqueue(this->GetEastToEastStart());
	LocationQueue.Enqueue(this->GetWestToNorthStart());
	LocationQueue.Enqueue(this->GetWestToEastStart());
	LocationQueue.Enqueue(this->GetWestToSouthStart());
	LocationQueue.Enqueue(this->GetWestToWestStart());
	
	while (!LocationQueue.IsEmpty())
	{
		UUMPathLocation* Popped = *LocationQueue.Peek();
		LocationQueue.Pop();
		for (UUMPathLocation *L2 : Popped->NextLocations)
		{
			double Distance;
			FVector b = L2->Location - Popped->Location;
			FVector a = PlayerLocation - Popped->Location;
			FVector proja_b = a.ProjectOnTo(b);
			// seems to work
			if (proja_b.GetUnsafeNormal() != b.GetUnsafeNormal() or proja_b.Length() > b.Length())
			{
				Distance = FMath::Min(FVector::Distance(Popped->Location, PlayerLocation), FVector::Distance(L2->Location, PlayerLocation));
			}
			else
			{
				Distance = (a - proja_b).Length();
			}
			
			if (Distance < Out || Out == -1) Out = Distance;
			LocationQueue.Enqueue(L2);
		}
	}
	
	return Out;
}

TArray<UUMPathLocation*> UUMMapModule::GetDirectionStartPoints(FVector Center, FVector Location)
{
	TArray<UUMPathLocation*> StartLocations;
	FVector Difference = Location - Center;
	if (FMath::Abs(Difference.X) > FMath::Abs(Difference.Y))
	{
		if (Difference.X > 0)
		{
			StartLocations.Add(this->GetNorthToNorthStart());
			StartLocations.Add(this->GetNorthToEastStart());
			StartLocations.Add(this->GetNorthToSouthStart());
			StartLocations.Add(this->GetNorthToWestStart());
		}
		else
		{
			StartLocations.Add(this->GetSouthToNorthStart());
			StartLocations.Add(this->GetSouthToEastStart());
			StartLocations.Add(this->GetSouthToSouthStart());
			StartLocations.Add(this->GetSouthToWestStart());
		}
	}
	else
	{
		if (Difference.Y > 0)
		{
			StartLocations.Add(this->GetEastToNorthStart());
			StartLocations.Add(this->GetEastToEastStart());
			StartLocations.Add(this->GetEastToSouthStart());
			StartLocations.Add(this->GetEastToWestStart());
		}
		else
		{
			StartLocations.Add(this->GetWestToNorthStart());
			StartLocations.Add(this->GetWestToEastStart());
			StartLocations.Add(this->GetWestToSouthStart());
			StartLocations.Add(this->GetWestToWestStart());
		}
	}
	return StartLocations;
}

UUMPathLocation* UUMMapModule::BuildLinearPath(TArray<FVector> Locations)
{
	UUMPathLocation* LastLocation = NewObject<UUMPathLocation>();
	Algo::Reverse(Locations);
	for (FVector V : Locations)
	{
		if (LastLocation->Location.IsZero())
		{
			LastLocation->Location = V;
		}
		else
		{
			UUMPathLocation* NewLocation = NewObject<UUMPathLocation>();
			NewLocation->Location = V;
			NewLocation->NextLocations.Add(LastLocation);
			LastLocation = NewLocation;
		}
	}
	return LastLocation;
}
