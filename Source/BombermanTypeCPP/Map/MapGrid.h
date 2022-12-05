// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapGrid.generated.h"


UCLASS()
class BOMBERMANTYPECPP_API AMapGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMapGrid();



	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<class UMapGridpoint*> Gridpoints;

	class UMapGridpoint* GetPointOnGrid(int32 x, int32 y);
	class UMapGridpoint* GetPointOnGrid(FIntPoint coord);

	TArray<class UMapGridpoint*>* GenerateMapGrid();

	FIntPoint GetSize() const;
	void SetSize(int32 x, int32 y);
	void SetSize(FIntPoint size);
	float GetGridDistance() const;
	void SetGridDistance(float gridDistance);
	FVector ConvertGridToWorld(FIntPoint coord) const;

	FVector GetClosestGridCenter(FVector Location);
	FIntPoint GetClosestGridPoint(FVector Location);

	FVector GetGridMiddle();

	TArray<FIntPoint> GetReachablePoints(FIntPoint coord, bool includePlayers = false);
	TArray<FIntPoint> GetSafeReachablePoints(FIntPoint coord, bool includePlayers = false);

	void AddBreakable();
	void RemoveBreakable();

	bool IsPointOnGrid(FIntPoint point);

	void AddBomb(FIntPoint point);
	void DeleteBomb(FIntPoint point);
	bool IsSafePoint(FIntPoint point);
	bool IsBombOnPoint(FIntPoint point);
	
	void AddPowerup(FIntPoint point);
	void DeletePowerup(FIntPoint point);
	int GetPowerupCount();

	int GetPlayerIndex();
	int RatePointForExplosion(FIntPoint point, int expRange);
	FIntPoint GetBestPointForExplosion(TArray<FIntPoint>& points, int expRange);
	TArray<FIntPoint> GetReachablePowerupList(TArray<FIntPoint>& points);

	TArray<FIntPoint> GetReachablePlayerList(TArray<FIntPoint>& points, int excludePlayer);
	TArray<FIntPoint> GetPointsOverseeing(FIntPoint point, int expRange);
	static TArray<FIntPoint> Intersection(TArray<FIntPoint> A, TArray<FIntPoint> B);

	void UpdatePlayerPosition(int character, FIntPoint point);

	int GetBreakableCount() const;

	TArray<FVector> GetFourCorners() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	
	FIntPoint Size;
	float GridDistance = 0;
	int BreakableCount = 0;
	int NextPlayerIndex = 0;
	TArray<FIntPoint> BombsPlaced;
	TArray<FIntPoint> PowerupsAvailable;
	TArray<FIntPoint> GetPointNeighbors(FIntPoint coord, bool includePlayers = false);
	TArray<FIntPoint> Players;

	void AddCharacter();
};
