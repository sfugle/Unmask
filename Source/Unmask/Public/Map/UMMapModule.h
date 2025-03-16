// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UMPathLocation.h"
#include "UObject/Object.h"
#include "UMMapModule.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class UNMASK_API UUMMapModule : public UObject
{
	GENERATED_BODY()
	UPROPERTY()
	UUMPathLocation* NorthToEastStart;
	UPROPERTY()
	UUMPathLocation* NorthToSouthStart;
	UPROPERTY()
	UUMPathLocation* NorthToWestStart;
	UPROPERTY()
	UUMPathLocation* NorthToNorthStart;
	UPROPERTY()
	UUMPathLocation* SouthToWestStart;
	UPROPERTY()
	UUMPathLocation* SouthToNorthStart;
	UPROPERTY()
	UUMPathLocation* SouthToEastStart;
	UPROPERTY()
	UUMPathLocation* SouthToSouthStart;
	UPROPERTY()
	UUMPathLocation* EastToSouthStart;
	UPROPERTY()
	UUMPathLocation* EastToWestStart;
	UPROPERTY()
	UUMPathLocation* EastToNorthStart;
	UPROPERTY()
	UUMPathLocation* EastToEastStart;
	UPROPERTY()
	UUMPathLocation* WestToNorthStart;
	UPROPERTY()
	UUMPathLocation* WestToEastStart;
	UPROPERTY()
	UUMPathLocation* WestToSouthStart;
	UPROPERTY()
	UUMPathLocation* WestToWestStart;
public:
	UUMMapModule() {}
	UFUNCTION(BlueprintCallable)
	static UUMMapModule* InitTestingMapModule();
	UFUNCTION(BlueprintCallable)
	static UUMMapModule* InitModule(int ID);
	UFUNCTION(BlueprintCallable)
	UUMPathLocation* GetNorthToEastStart() { return NorthToEastStart; }
	UFUNCTION(BlueprintCallable)
	UUMPathLocation* GetNorthToSouthStart() { return NorthToSouthStart; }
	UFUNCTION(BlueprintCallable)
	UUMPathLocation* GetNorthToWestStart() { return NorthToWestStart; }
	UFUNCTION(BlueprintCallable)
	UUMPathLocation* GetNorthToNorthStart() { return NorthToNorthStart; }
	UFUNCTION(BlueprintCallable)
	UUMPathLocation* GetSouthToWestStart() { return SouthToWestStart; }
	UFUNCTION(BlueprintCallable)
	UUMPathLocation* GetSouthToNorthStart() { return SouthToNorthStart; }
	UFUNCTION(BlueprintCallable)
	UUMPathLocation* GetSouthToEastStart() { return SouthToEastStart; }
	UFUNCTION(BlueprintCallable)
	UUMPathLocation* GetSouthToSouthStart() { return SouthToSouthStart; }
	UFUNCTION(BlueprintCallable)
	UUMPathLocation* GetEastToSouthStart() { return EastToSouthStart; }
	UFUNCTION(BlueprintCallable)
	UUMPathLocation* GetEastToWestStart() { return EastToWestStart; }
	UFUNCTION(BlueprintCallable)
	UUMPathLocation* GetEastToNorthStart() { return EastToNorthStart; }
	UFUNCTION(BlueprintCallable)
	UUMPathLocation* GetEastToEastStart() { return EastToEastStart; }
	UFUNCTION(BlueprintCallable)
	UUMPathLocation* GetWestToNorthStart() { return WestToNorthStart; }
	UFUNCTION(BlueprintCallable)
	UUMPathLocation* GetWestToEastStart() { return WestToEastStart; }
	UFUNCTION(BlueprintCallable)
	UUMPathLocation* GetWestToSouthStart() { return WestToSouthStart; }
	UFUNCTION(BlueprintCallable)
	UUMPathLocation* GetWestToWestStart() { return WestToWestStart; }
	UFUNCTION(BlueprintCallable)
	double DistanceToClosestPath(FVector PlayerLocation);
	UFUNCTION(BlueprintCallable)
	TArray<UUMPathLocation*> GetDirectionStartPoints(FVector Center, FVector Location);
	UFUNCTION(BlueprintCallable)
	static UUMPathLocation* BuildLinearPath(TArray<FVector> Locations);
};
