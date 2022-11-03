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

	void SetGridDistance(float gridDistance);
	FVector ConvertGridToWorld(FIntPoint coord);

	FVector GetClosestGridCenter(FVector Location);
	FIntPoint GetClosestGridPoint(FVector Location);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	
	FIntPoint Size;
	float GridDistance = 0;
};
